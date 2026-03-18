#include "qml/playerpage.hpp"
#include "deezer/apiresponse.hpp"
#include "deezer/cypher.hpp"
#include "deezer/deezerclient.hpp"
#include "deezer/objects/mediaurl.hpp"

PlayerPage::PlayerPage(QObject *parent)
	: QObject(parent),
	mHttp(this),
	mMediaPlayer(MediaFormat::LowQuality, this)
{
	refreshUserData();
}

auto PlayerPage::userImage() const -> const QImage &
{
	return mUserImage;
}

void PlayerPage::enqueueTrack(const qint64 trackId)
{
	mMediaPlayer.enqueueTrack(trackId);
}

void PlayerPage::play()
{
	mMediaPlayer.play();
}

void PlayerPage::refreshUserData() const
{
	const ApiResponse *response = DeezerClient::instance()->gw().userData();

	connect(response, &ApiResponse::finished,
		this, &PlayerPage::onUserDataResponse);
}

void PlayerPage::onUserDataResponse()
{
	auto *response = qobject_cast<ApiResponse *>(sender());
	if (!response->isValid())
	{
		qWarning() << "Failed to get user data:" << response->errorString();
		response->deleteLater();
		return;
	}

	mUserData = response->value<UserData>();
	mMediaPlayer.setUserData(mUserData);

	const QNetworkRequest request(mUserData.userPictureUrl());
	const QNetworkReply *reply = mHttp.get(request);

	connect(reply, &QNetworkReply::finished,
		this, &PlayerPage::onUserPictureResponse);
}

void PlayerPage::onUserPictureResponse()
{
	auto *reply = qobject_cast<QNetworkReply *>(sender());
	if (reply->error() != QNetworkReply::NoError)
	{
		qWarning() << "Failed to get user data:" << reply->errorString();
		reply->deleteLater();
		return;
	}

	mUserImage = QImage::fromData(reply->readAll(), "jpg");
	emit userImageChanged();
}
