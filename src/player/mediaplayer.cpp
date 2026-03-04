#include "player/mediaplayer.hpp"
#include "deezer/apiresponse.hpp"
#include "deezer/cypher.hpp"
#include "deezer/deezerclient.hpp"
#include "deezer/objects/mediaurl.hpp"
#include "util/enumserializer.hpp"

#include <QMediaDevices>

MediaPlayer::MediaPlayer(QObject *parent)
	: QObject(parent),
	mAudioDecoder(this),
	mAudioSink({}, this),
	mDecodedAudioBuffer(&mDecodedAudioData, this)
{
	connect(&mAudioDecoder, &QAudioDecoder::bufferReady,
		this, &MediaPlayer::onAudioDecoderBufferReady);

	connect(&mAudioDecoder, qOverload<QAudioDecoder::Error>(&QAudioDecoder::error),
		this, &MediaPlayer::onAudioDecoderError);

	logAudioConfig();
}

void MediaPlayer::enqueue(const UserData &userData,
	const qint64 trackId, const MediaFormat mediaFormat)
{
	mCurrentUserData = userData;
	mQueue.enqueue({
		.trackId = trackId,
		.mediaFormat = mediaFormat,
	});

	DeezerClient *client = DeezerClient::instance();
	const ApiResponse *response = client->gw().songData(userData, trackId);

	connect(response, &ApiResponse::finished,
		this, &MediaPlayer::onSongData);
}

void MediaPlayer::logAudioConfig() const
{
	const QAudioDevice device = QMediaDevices::defaultAudioOutput();
	qDebug() << "Playback device:" << device.description();
	qDebug() << "Channel config:" << EnumSerializer::toString(device.channelConfiguration());

	const QAudioFormat format = mAudioSink.format();
	qDebug().nospace().noquote()
		<< "Sample rate: " << format.sampleRate() << " Hz"
		<< " (" << EnumSerializer::toString(format.sampleFormat()) << ")";
}

void MediaPlayer::onAudioDecoderBufferReady()
{
	const QAudioBuffer buffer = mAudioDecoder.read();
	mDecodedAudioData.append(buffer.constData<const char>(), buffer.byteCount());
}

void MediaPlayer::onAudioDecoderError([[maybe_unused]] const QAudioDecoder::Error error) const
{
	qWarning() << "Audio decoder error:" << mAudioDecoder.errorString();
}

void MediaPlayer::onSongData()
{
	auto *response = qobject_cast<ApiResponse *>(sender());
	if (!response->isValid())
	{
		qWarning() << "Failed to get song data:" << response->errorString();
		response->deleteLater();
		return;
	}

	const auto songData = response->value<SongData>();
	response->deleteLater();

	DeezerClient *client = DeezerClient::instance();
	response = client->media().url(mCurrentUserData, songData, mQueue.head().mediaFormat);

	connect(response, &ApiResponse::finished,
		this, &MediaPlayer::onMediaUrl);
}

void MediaPlayer::onMediaUrl()
{
	auto *const response = qobject_cast<ApiResponse *>(sender());
	if (!response->isValid())
	{
		qWarning() << "Failed to get media url:" << response->errorString();
		response->deleteLater();
		return;
	}

	const auto mediaUrl = response->value<MediaUrl>();
	response->deleteLater();

	const QNetworkRequest request(mediaUrl.sources().at(0).url());
	const QNetworkReply *reply = mHttp.get(request);

	connect(reply, &QNetworkReply::finished,
		this, &MediaPlayer::onMediaDownloaded);
}

void MediaPlayer::onMediaDownloaded()
{
	auto *const reply = qobject_cast<QNetworkReply *>(sender());
	if (reply->error() != QNetworkReply::NoError)
	{
		qWarning() << "Failed to download media:" << reply->errorString();
		reply->deleteLater();
		return;
	}

	const QByteArray data = reply->readAll();
	reply->deleteLater();

	const QByteArray key = Cypher::generateKey(mQueue.head().trackId);
	const IV iv = Cypher::generateIv();

	mAudioBuffer.close();

	mAudioData = Cypher::decrypt(key, iv, data);

	if (mQueue.head().mediaFormat != MediaFormat::Lossless)
	{
		constexpr std::array<char, 10> id3Header = {
			0x49, 0x44, 0x33,       // "ID3" identifier
			0x03, 0x00,             // Version (v2.3.0)
			0x00,                   // Flags
			0x00, 0x00, 0x00, 0x00, // Tag size
		};
		mAudioData.prepend(id3Header);
	}

	mAudioBuffer.setBuffer(&mAudioData);
	mAudioDecoder.setSourceDevice(&mAudioBuffer);
	mAudioBuffer.open(QIODevice::ReadOnly);

	mAudioDecoder.start();

	mDecodedAudioBuffer.close();
	mDecodedAudioData.clear();
	mDecodedAudioBuffer.open(QIODevice::ReadOnly);

	mAudioSink.start(&mDecodedAudioBuffer);
}
