#pragma once

#include "deezer/enums/mediaformat.hpp"
#include "deezer/objects/userdata.hpp"
#include "player/queueitem.hpp"

#include <QAudioDecoder>
#include <QAudioSink>
#include <QBuffer>
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

	QAudioDecoder mAudioDecoder;
	QBuffer mAudioDecoderSource;

	QAudioSink mAudioSink;
	QIODevice *mAudioSinkData;

	UserData mCurrentUserData;
	MediaFormat mMediaFormat;
	QQueue<QueueItem> mQueue;

	void logAudioConfig() const;

	void playHead();

	void onAudioDecoderBufferReady() const;

	void onAudioDecoderError(QAudioDecoder::Error error) const;

	void onAudioSinkStateChanged(QtAudio::State state) const;

	void onSongData();

	void onMediaUrl();

	void onMediaDownloaded();
};
