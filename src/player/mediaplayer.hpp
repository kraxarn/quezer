#pragma once

#include "deezer/enums/mediaformat.hpp"
#include "deezer/objects/userdata.hpp"

#include <QAudioDecoder>
#include <QAudioSink>
#include <QBuffer>
#include <QNetworkAccessManager>
#include <QQueue>

class MediaPlayer final : public QObject
{
	Q_OBJECT

public:
	explicit MediaPlayer(QObject *parent);

	void enqueue(qint64 trackId, MediaFormat mediaFormat);

	void setUserData(const UserData &userData);

private:
	struct QueueItem final
	{
		qint64 trackId;
		MediaFormat mediaFormat;
		QUrl mediaUrl;
		QByteArray audioData;
	};

	QNetworkAccessManager mHttp;
	QAudioDecoder mAudioDecoder;
	QAudioSink mAudioSink;

	QBuffer mAudioBuffer;
	QByteArray mDecodedAudioData;
	QBuffer mDecodedAudioBuffer;

	UserData mCurrentUserData;
	QQueue<QueueItem> mQueue;

	void logAudioConfig() const;

	void onAudioDecoderBufferReady();

	void onAudioDecoderError(QAudioDecoder::Error error) const;

	void onSongData();

	void onMediaUrl();

	void onMediaDownloaded();
};
