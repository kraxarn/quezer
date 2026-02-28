#include "qml/playerpage.hpp"
#include "deezer/apiresponse.hpp"
#include "deezer/cypher.hpp"
#include "deezer/deezerclient.hpp"
#include "deezer/objects/mediaurl.hpp"

PlayerPage::PlayerPage(QObject *parent)
	: QObject(parent),
	mHttp(this),
	mMediaPlayer(this),
	mAudioOutput(this)
{
	mMediaPlayer.setAudioOutput(&mAudioOutput);
	mAudioBuffer.setBuffer(&mAudioData);
	mMediaPlayer.setSourceDevice(&mAudioBuffer);

	connect(&mMediaPlayer, &QMediaPlayer::errorOccurred,
		this, &PlayerPage::onMediaPlayerErrorOccured);

	refreshUserData();
}

auto PlayerPage::userImage() const -> const QImage &
{
	return mUserImage;
}

void PlayerPage::play(const qint64 trackId)
{
	mCurrentTrackId = trackId;

	DeezerClient *client = DeezerClient::instance();
	const ApiResponse *response = client->gw().songData(mUserData, trackId);

	connect(response, &ApiResponse::finished,
		this, &PlayerPage::onSongData);
}

void PlayerPage::refreshUserData()
{
	const ApiResponse *response = DeezerClient::instance()->gw().userData();

	connect(response, &ApiResponse::finished,
		this, &PlayerPage::onUserDataResponse);
}

void PlayerPage::onMediaPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString)
{
	qWarning() << "Media player error:" << errorString;
}

void PlayerPage::onUserDataResponse()
{
	const auto response = qobject_cast<ApiResponse *>(sender());
	if (!response->isValid())
	{
		qWarning() << "Failed to get user data:" << response->errorString();
		response->deleteLater();
		return;
	}

	mUserData = response->value<UserData>();

	const QNetworkRequest request(mUserData.userPictureUrl());
	const QNetworkReply *reply = mHttp.get(request);

	connect(reply, &QNetworkReply::finished,
		this, &PlayerPage::onUserPictureResponse);
}

void PlayerPage::onUserPictureResponse()
{
	const auto reply = qobject_cast<QNetworkReply *>(sender());
	if (reply->error() != QNetworkReply::NoError)
	{
		qWarning() << "Failed to get user data:" << reply->errorString();
		reply->deleteLater();
		return;
	}

	mUserImage = QImage::fromData(reply->readAll(), "jpg");
	emit userImageChanged();
}

void PlayerPage::onSongData() const
{
	auto response = qobject_cast<ApiResponse *>(sender());
	if (!response->isValid())
	{
		qWarning() << "Failed to play track:" << response->errorString();
		response->deleteLater();
		return;
	}

	const SongData songData = response->value<SongData>();
	response->deleteLater();

	DeezerClient *client = DeezerClient::instance();
	response = client->media().url(mUserData, songData, MediaFormat::LowQuality);

	connect(response, &ApiResponse::finished,
		this, &PlayerPage::onMediaUrl);
}

void PlayerPage::onMediaUrl()
{
	const auto response = qobject_cast<ApiResponse *>(sender());
	if (!response->isValid())
	{
		qWarning() << "Failed to play track:" << response->errorString();
		response->deleteLater();
		return;
	}

	const MediaUrl mediaUrl = response->value<MediaUrl>();
	response->deleteLater();

	const QNetworkRequest request(mediaUrl.sources().at(0).url());
	const QNetworkReply *reply = mHttp.get(request);

	connect(reply, &QNetworkReply::finished,
		this, &PlayerPage::onMediaDownloaded);
}

void PlayerPage::onMediaDownloaded()
{
	const auto reply = qobject_cast<QNetworkReply *>(sender());
	if (reply->error() != QNetworkReply::NoError)
	{
		qWarning() << "Failed to play track:" << reply->errorString();
		reply->deleteLater();
		return;
	}

	const QByteArray data = reply->readAll();
	reply->deleteLater();

	const QByteArray key = Cypher::generateKey(mCurrentTrackId);
	const IV iv = Cypher::generateIv();

	mMediaPlayer.stop();
	{
		mAudioData = Cypher::decrypt(key, iv, data);
	}
	mMediaPlayer.play();
}
