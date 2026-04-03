#include "player/mediaplayer.hpp"
#include "deezer/apiresponse.hpp"
#include "deezer/cypher.hpp"
#include "deezer/deezerclient.hpp"
#include "deezer/objects/mediaurl.hpp"
#include "util/enumserializer.hpp"

#include <QAudioDevice>
#include <QAudioOutput>
#include <QBuffer>

MediaPlayer::MediaPlayer(const MediaFormat mediaFormat, QObject *parent)
	: QObject(parent),
	mHttp(this),
	mMediaPlayer(this),
	mAudioOutput(this),
	mAudioBuffer(this),
	mMediaFormat(mediaFormat)
{
	mMediaPlayer.setAudioOutput(&mAudioOutput);

	connect(&mMediaPlayer, &QMediaPlayer::errorOccurred,
		this, &MediaPlayer::onMediaPlayerErrorOccurred);

	connect(&mMediaPlayer, &QMediaPlayer::mediaStatusChanged,
		this, &MediaPlayer::onMediaPlayerMediaStatusChanged);

	connect(&mMediaPlayer, &QMediaPlayer::playbackStateChanged,
		this, &MediaPlayer::onMediaPlayerPlaybackStateChanged);

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
	if (mMediaPlayer.isPlaying())
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

auto MediaPlayer::metaData() const -> const MetaData *
{
	return mMetaData.data();
}

void MediaPlayer::logAudioConfig() const
{
	const QAudioOutput *audioOutput = mMediaPlayer.audioOutput();
	if (audioOutput == nullptr)
	{
		return;
	}

	const QAudioDevice &device = audioOutput->device();
	qDebug() << "Playback device:" << device.description();
	qDebug() << "Channel config:" << EnumSerializer::toString(device.channelConfiguration());
}

void MediaPlayer::playHead()
{
	QueueItem &item = mQueue.head();

	const QString filename = QStringLiteral("%1-%2.%3")
		.arg(item.songData.sngId())
		.arg(static_cast<quint8>(item.mediaFormat))
		.arg(item.mediaFormat == MediaFormat::Lossless
			? QStringLiteral("flac")
			: QStringLiteral("mp3"));

	qDebug() << "Playing:" << filename;

	mAudioBuffer.close();
	mAudioBuffer.setBuffer(&item.audioData);
	mAudioBuffer.open(QIODevice::ReadOnly);

	mMediaPlayer.setSourceDevice(&mAudioBuffer, filename);
	mMediaPlayer.play();

	mMetaData = new MetaData(item.songData, filename);
	emit metaDataChanged();
}

void MediaPlayer::onMediaPlayerErrorOccurred(const QMediaPlayer::Error error,
	const QString &errorString)
{
	qWarning().nospace()
		<< "Media player error: " << errorString
		<< " (" << error << ")";
}

void MediaPlayer::onMediaPlayerMediaStatusChanged(const QMediaPlayer::MediaStatus status)
{
	qWarning().nospace()
		<< "Media status: " << EnumSerializer::toString(status)
		<< " (" << status << ")";
}

void MediaPlayer::onMediaPlayerPlaybackStateChanged(const QMediaPlayer::PlaybackState state)
{
	qWarning().nospace()
		<< "Playback state: " << EnumSerializer::toString(state)
		<< " (" << state << ")";
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

	item.status = QueueItemStatus::Ready;

	if (!mMediaPlayer.isPlaying())
	{
		playHead();
	}
}
