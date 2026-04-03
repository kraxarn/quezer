#pragma once

#include "deezer/apiresponse.hpp"
#include "deezer/enums/mediaformat.hpp"
#include "deezer/objects/userdata.hpp"
#include "player/mediaplayer.hpp"
#include "player/metadata.hpp"

#include <QImage>
#include <QNetworkAccessManager>
#include <QObject>
#include <qqmlintegration.h>

class PlayerPage : public QObject
{
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(QImage userImage READ userImage NOTIFY userImageChanged)
	Q_PROPERTY(const MetaData * metaData READ metaData NOTIFY metaDataChanged)

public:
	explicit PlayerPage(QObject *parent = nullptr);

	[[nodiscard]]
	auto userImage() const -> const QImage &;

	[[nodiscard]]
	auto metaData() const -> const MetaData *;

	Q_INVOKABLE void enqueueTrack(qint64 trackId);

	Q_INVOKABLE void play();

signals:
	void userImageChanged();

	void metaDataChanged();

private:
	QNetworkAccessManager mHttp;
	MediaPlayer mMediaPlayer;
	QImage mUserImage;
	UserData mUserData;

	void refreshUserData() const;

	void onUserDataResponse();

	void onUserPictureResponse();
};
