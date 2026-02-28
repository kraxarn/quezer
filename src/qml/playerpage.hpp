#pragma once

#include "deezer/apiresponse.hpp"
#include "deezer/objects/userdata.hpp"

#include <QImage>
#include <QNetworkAccessManager>
#include <QObject>
#include <qqmlintegration.h>

class PlayerPage : public QObject
{
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(QImage userImage READ userImage NOTIFY userImageChanged)

public:
	explicit PlayerPage(QObject *parent = nullptr);

	[[nodiscard]]
	auto userImage() const -> const QImage &;

signals:
	void userImageChanged();

private:
	QNetworkAccessManager mHttp;

	QImage mUserImage;

	void refreshUserData();

	void onUserDataResponse();

	void onUserPictureResponse();
};
