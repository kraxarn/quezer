#pragma once

#include "deezer/enums/mediaformat.hpp"
#include "deezer/objects/userdata.hpp"

#include <QAudioDecoder>
#include <QAudioSink>
#include <QBuffer>
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
	QAudioDecoder mAudioDecoder;
	QAudioSink mAudioSink;

	QByteArray mAudioData;
	QBuffer mAudioBuffer;

	QByteArray mDecodedAudioData;
	QBuffer mDecodedAudioBuffer;

	UserData mCurrentUserData;
	qint64 mCurrentTrackId;
	MediaFormat mCurrentMediaFormat;

	void logAudioConfig() const;

	[[nodiscard]]
	static auto channelConfigString(QAudioFormat::ChannelConfig channelConfig) -> QString;

	[[nodiscard]]
	static auto sampleFormatString(QAudioFormat::SampleFormat sampleFormat) -> QString;

	void onAudioDecoderBufferReady();

	void onAudioDecoderError(QAudioDecoder::Error error) const;

	void onSongData();

	void onMediaUrl();

	void onMediaDownloaded();
};
