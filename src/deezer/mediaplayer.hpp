#pragma once

#include "deezer/enums/mediaformat.hpp"
#include "deezer/objects/userdata.hpp"

#include <QAudioOutput>
#include <QBuffer>
#include <QMediaPlayer>
#include <QNetworkAccessManager>

class MediaPlayer final : public QObject
{
	Q_OBJECT

public:
	explicit MediaPlayer(QObject *parent);

	void playTrack(const UserData &userData,
		qint64 trackId, MediaFormat mediaFormat);

private:
	QNetworkAccessManager mHttp;
	QMediaPlayer mMediaPlayer;
	QAudioOutput mAudioOutput;
	QByteArray mAudioData;
	QBuffer mAudioBuffer;

	UserData mCurrentUserData;
	qint64 mCurrentTrackId;
	MediaFormat mCurrentMediaFormat;

	static void onMediaPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString);

	void onSongData();

	void onMediaUrl();

	void onMediaDownloaded();
};
