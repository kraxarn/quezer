#include "qml/playerpage.hpp"
#include "deezer/apiresponse.hpp"
#include "deezer/deezerclient.hpp"

PlayerPage::PlayerPage(QObject *parent)
	: QObject(parent),
	mHttp(this)
{
	refreshUserData();
}

auto PlayerPage::userImage() const -> const QImage &
{
	return mUserImage;
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

	const auto userData = response->value<UserData>();

	const QNetworkRequest request(userData.userPictureUrl());
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
