#include "player/mediaplayer.hpp"
#include "deezer/apiresponse.hpp"
#include "deezer/cypher.hpp"
#include "deezer/deezerclient.hpp"
#include "deezer/objects/mediaurl.hpp"
#include "util/enumserializer.hpp"

#include <QMediaDevices>

MediaPlayer::MediaPlayer(const MediaFormat mediaFormat, QObject *parent)
	: QObject(parent),
	mAudioDecoder(this),
	mAudioSink({}, this),
	mAudioSinkData(mAudioSink.start()),
	mMediaFormat(mediaFormat)
{
	mAudioDecoder.setSourceDevice(&mAudioDecoderSource);

	connect(&mAudioDecoder, &QAudioDecoder::bufferReady,
		this, &MediaPlayer::onAudioDecoderBufferReady);

	connect(&mAudioDecoder, qOverload<QAudioDecoder::Error>(&QAudioDecoder::error),
		this, &MediaPlayer::onAudioDecoderError);

	connect(&mAudioSink, &QAudioSink::stateChanged,
		this, &MediaPlayer::onAudioSinkStateChanged);

	logAudioConfig();
}

void MediaPlayer::enqueueTrack(const qint64 trackId)
{
	DeezerClient *client = DeezerClient::instance();
	const ApiResponse *response = client->gw().songData(mCurrentUserData, trackId);

	connect(response, &ApiResponse::finished,
		this, &MediaPlayer::onSongData);
}

void MediaPlayer::setUserData(const UserData &userData)
{
	mCurrentUserData = userData;
}

void MediaPlayer::play()
{
	if (mAudioSink.state() == QtAudio::ActiveState)
	{
		return;
	}

	if (mQueue.isEmpty())
	{
		connect(this, &MediaPlayer::queueItemAdded,
			this, &MediaPlayer::play);
		return;
	}

	QueueItem &item = mQueue.head();

	if (item.status == QueueItemStatus::Ready)
	{
		playHead();
		return;
	}

	if (item.status == QueueItemStatus::Buffering)
	{
		return;
	}

	item.status = QueueItemStatus::Buffering;

	const ApiResponse *response = DeezerClient::instance()->media()
		.url(mCurrentUserData, item.songData, item.mediaFormat);

	connect(response, &ApiResponse::finished,
		this, &MediaPlayer::onMediaUrl);
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

void MediaPlayer::playHead()
{
	const QueueItem &item = mQueue.head();

	QIODevice *source = mAudioDecoder.sourceDevice();

	source->close();
	source->open(QIODevice::WriteOnly | QIODevice::Truncate);
	source->write(item.audioData);

	source->close();
	source->open(QIODevice::ReadOnly);
	source->seek(0);

	mAudioDecoder.start();
}

void MediaPlayer::onAudioDecoderBufferReady() const
{
	const QAudioBuffer buffer = mAudioDecoder.read();
	mAudioSinkData->write(buffer.constData<const char>(), buffer.byteCount());
}

void MediaPlayer::onAudioDecoderError(const QAudioDecoder::Error error) const
{
	qWarning().nospace()
		<< "Audio decoder error: " << mAudioDecoder.errorString()
		<< " (" << error << ")";
}

void MediaPlayer::onAudioSinkStateChanged(const QtAudio::State state) const
{
	if (mAudioSink.error() == QtAudio::NoError)
	{
		qDebug() << "Audio sink:" << EnumSerializer::toString(state);
	}
	else
	{
		qCritical().nospace()
			<< "Audio sink: " << EnumSerializer::toString(state)
			<< " (" << EnumSerializer::toString(mAudioSink.error()) << ")";
	}
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

	auto songData = response->value<SongData>();
	response->deleteLater();

	mQueue.enqueue({
		.status = QueueItemStatus::Waiting,
		.songData = std::move(songData),
		.mediaFormat = mMediaFormat,
	});

	emit queueItemAdded();
}

void MediaPlayer::onMediaUrl()
{
	const auto response = qobject_cast<ApiResponse *>(sender());
	if (!response->isValid())
	{
		qCritical() << "Failed to get media url:" << response->errorString();
		response->deleteLater();
		return;
	}

	const auto mediaUrl = response->value<MediaUrl>();
	response->deleteLater();

	const QUrl &url = mediaUrl.sources().at(0).url();
	const QNetworkRequest request(url);
	const QNetworkReply *reply = mHttp.get(request);

	connect(reply, &QNetworkReply::finished,
		this, &MediaPlayer::onMediaDownloaded);
}

void MediaPlayer::onMediaDownloaded()
{
	const auto reply = qobject_cast<QNetworkReply *>(sender());
	if (reply->error() != QNetworkReply::NoError)
	{
		qCritical() << "Failed to download media:" << reply->errorString();
		reply->deleteLater();
		return;
	}

	const QByteArray data = reply->readAll();
	reply->deleteLater();

	QueueItem &item = mQueue.head();
	const QByteArray key = Cypher::generateKey(item.songData.sngId());
	const IV iv = Cypher::generateIv();
	item.audioData = Cypher::decrypt(key, iv, data);

	if (item.mediaFormat != MediaFormat::Lossless)
	{
		constexpr std::array<char, 10> id3Header = {
			0x49, 0x44, 0x33,       // "ID3" identifier
			0x03, 0x00,             // Version (v2.3.0)
			0x00,                   // Flags
			0x00, 0x00, 0x00, 0x00, // Tag size
		};
		item.audioData.prepend(id3Header);
	}

	item.status = QueueItemStatus::Ready;

	if (mAudioSink.state() == QtAudio::IdleState)
	{
		playHead();
	}
}
