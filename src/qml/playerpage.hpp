#pragma once

#include "deezer/apiresponse.hpp"
#include "deezer/enums/mediaformat.hpp"
#include "deezer/objects/userdata.hpp"

#include <QAudioOutput>
#include <QBuffer>
#include <QImage>
#include <QMediaPlayer>
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

	Q_INVOKABLE void play(qint64 trackId);

signals:
	void userImageChanged();

private:
	QNetworkAccessManager mHttp;
	QMediaPlayer mMediaPlayer;
	QAudioOutput mAudioOutput;
	QByteArray mAudioData;
	QBuffer mAudioBuffer;
	QImage mUserImage;

	UserData mUserData;
	qint64 mCurrentTrackId;
	MediaFormat mCurrentMediaFormat;

	void refreshUserData();

	void onMediaPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString);

	void onUserDataResponse();

	void onUserPictureResponse();

	void onSongData();

	void onMediaUrl();

	void onMediaDownloaded();
};
