#pragma once

#include "deezer/enums/mediaformat.hpp"
#include "deezer/objects/userdata.hpp"
#include "player/queueitem.hpp"

#include <QAudioOutput>
#include <QBuffer>
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QQueue>

class MediaPlayer final : public QObject
{
	Q_OBJECT

public:
	MediaPlayer(MediaFormat mediaFormat, QObject *parent);

	void enqueueTrack(qint64 trackId);

	void setUserData(const UserData &userData);

	void play();

signals:
	void queueItemAdded();

private:
	QNetworkAccessManager mHttp;
	QMediaPlayer mMediaPlayer;
	QAudioOutput mAudioOutput;
	QBuffer mAudioBuffer;

	UserData mCurrentUserData;
	MediaFormat mMediaFormat;
	QQueue<QueueItem> mQueue;

	void logAudioConfig() const;

	void playHead();

	static void onMediaPlayerErrorOccurred(QMediaPlayer::Error error,
		const QString &errorString);

	static void onMediaPlayerMediaStatusChanged(QMediaPlayer::MediaStatus status);

	static void onMediaPlayerPlaybackStateChanged(QMediaPlayer::PlaybackState state);

	void onSongData();

	void onMediaUrl();

	void onMediaDownloaded();
};
