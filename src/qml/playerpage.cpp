#include "qml/playerpage.hpp"
#include "deezer/apiresponse.hpp"
#include "deezer/cypher.hpp"
#include "deezer/deezerclient.hpp"
#include "deezer/objects/mediaurl.hpp"

PlayerPage::PlayerPage(QObject *parent)
	: QObject(parent),
	mHttp(this),
	mMediaPlayer(this)
{
	refreshUserData();
}

auto PlayerPage::userImage() const -> const QImage &
{
	return mUserImage;
}

void PlayerPage::enqueue(const qint64 trackId)
{
	mMediaPlayer.enqueue(mUserData, trackId, MediaFormat::LowQuality);
}

void PlayerPage::refreshUserData()
{
	const ApiResponse *response = DeezerClient::instance()->gw().userData();

	connect(response, &ApiResponse::finished,
		this, &PlayerPage::onUserDataResponse);
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
