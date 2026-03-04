#pragma once

#include <QAudioFormat>

class EnumSerializer
{
public:
	[[nodiscard]]
	static auto toString(QAudioFormat::ChannelConfig channelConfig) -> QString;

	[[nodiscard]]
	static auto toString(QAudioFormat::SampleFormat sampleFormat) -> QString;

private:
	EnumSerializer() = delete;
};
