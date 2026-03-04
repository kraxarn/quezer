#pragma once

#include "deezer/apiresponse.hpp"
#include "deezer/enums/mediaformat.hpp"
#include "deezer/objects/userdata.hpp"
#include "player/mediaplayer.hpp"

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

	Q_INVOKABLE void enqueue(qint64 trackId);

signals:
	void userImageChanged();

private:
	QNetworkAccessManager mHttp;
	MediaPlayer mMediaPlayer;
	QImage mUserImage;
	UserData mUserData;

	void refreshUserData();

	void onUserDataResponse();

	void onUserPictureResponse();
};
