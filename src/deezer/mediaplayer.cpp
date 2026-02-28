#include "deezer/mediaplayer.hpp"
#include "deezer/apiresponse.hpp"
#include "deezer/cypher.hpp"
#include "deezer/deezerclient.hpp"
#include "deezer/objects/mediaurl.hpp"

MediaPlayer::MediaPlayer(QObject *parent)
	: QObject(parent),
	mMediaPlayer(this),
	mAudioOutput(this),
	mCurrentUserData(UserData()),
	mCurrentTrackId(0),
	mCurrentMediaFormat(MediaFormat::LowQuality)
{
	mMediaPlayer.setAudioOutput(&mAudioOutput);

	connect(&mMediaPlayer, &QMediaPlayer::errorOccurred,
		this, &MediaPlayer::onMediaPlayerErrorOccurred);
}

void MediaPlayer::playTrack(const UserData &userData,
	const qint64 trackId, const MediaFormat mediaFormat)
{
	mCurrentUserData = userData;
	mCurrentTrackId = trackId;
	mCurrentMediaFormat = mediaFormat;

	DeezerClient *client = DeezerClient::instance();
	const ApiResponse *response = client->gw().songData(userData, trackId);

	connect(response, &ApiResponse::finished,
		this, &MediaPlayer::onSongData);
}

void MediaPlayer::onMediaPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString)
{
	qWarning() << "Media player error:" << errorString;
}

void MediaPlayer::onSongData()
{
	auto response = qobject_cast<ApiResponse *>(sender());
	if (!response->isValid())
	{
		qWarning() << "Failed to get song data:" << response->errorString();
		response->deleteLater();
		return;
	}

	const SongData songData = response->value<SongData>();
	response->deleteLater();

	DeezerClient *client = DeezerClient::instance();
	response = client->media().url(mCurrentUserData, songData, mCurrentMediaFormat);

	connect(response, &ApiResponse::finished,
		this, &MediaPlayer::onMediaUrl);
}

void MediaPlayer::onMediaUrl()
{
	const auto response = qobject_cast<ApiResponse *>(sender());
	if (!response->isValid())
	{
		qWarning() << "Failed to get media url:" << response->errorString();
		response->deleteLater();
		return;
	}

	const MediaUrl mediaUrl = response->value<MediaUrl>();
	response->deleteLater();

	const QNetworkRequest request(mediaUrl.sources().at(0).url());
	const QNetworkReply *reply = mHttp.get(request);

	connect(reply, &QNetworkReply::finished,
		this, &MediaPlayer::onMediaDownloaded);
}

void MediaPlayer::onMediaDownloaded()
{
	const auto reply = qobject_cast<QNetworkReply *>(sender());
	if (reply->error() != QNetworkReply::NoError)
	{
		qWarning() << "Failed to download media:" << reply->errorString();
		reply->deleteLater();
		return;
	}

	const QByteArray data = reply->readAll();
	reply->deleteLater();

	const QByteArray key = Cypher::generateKey(mCurrentTrackId);
	const IV iv = Cypher::generateIv();

	mMediaPlayer.stop();
	mAudioBuffer.close();

	mAudioData = Cypher::decrypt(key, iv, data);
	mAudioBuffer.setBuffer(&mAudioData);

	mMediaPlayer.setSourceDevice(&mAudioBuffer,QStringLiteral("%1.%2")
		.arg(mCurrentTrackId)
		.arg(mCurrentMediaFormat == MediaFormat::Lossless
			? QStringLiteral("flac")
			: QStringLiteral("mp3")));

	mAudioBuffer.open(QIODevice::ReadOnly);
	mMediaPlayer.play();
}
