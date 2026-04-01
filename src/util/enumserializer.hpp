#pragma once

#include <QAudioFormat>
#include <QMediaPlayer>
#include <QtAudio>

class EnumSerializer
{
public:
	EnumSerializer() = delete;

	[[nodiscard]]
	static auto toString(QAudioFormat::ChannelConfig channelConfig) -> QString;

	[[nodiscard]]
	static auto toString(QAudioFormat::SampleFormat sampleFormat) -> QString;

	[[nodiscard]]
	static auto toString(QtAudio::State state) -> QString;

	[[nodiscard]]
	static auto toString(QtAudio::Error error) -> QString;

	[[nodiscard]]
	static auto toString(QMediaPlayer::MediaStatus status) -> QString;

	[[nodiscard]]
	static auto toString(QMediaPlayer::PlaybackState state) -> QString;
};
