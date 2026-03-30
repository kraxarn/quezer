#include "util/enumserializer.hpp"

auto EnumSerializer::toString(QAudioFormat::ChannelConfig channelConfig) -> QString
{
	switch (channelConfig)
	{
		case QAudioFormat::ChannelConfigUnknown:
			return QStringLiteral("Unknown");

		case QAudioFormat::ChannelConfigMono:
			return QStringLiteral("Mono");

		case QAudioFormat::ChannelConfigStereo:
			return QStringLiteral("Stereo");

		case QAudioFormat::ChannelConfig2Dot1:
			return QStringLiteral("2.1");

		case QAudioFormat::ChannelConfig3Dot0:
			return QStringLiteral("3.0");

		case QAudioFormat::ChannelConfig3Dot1:
			return QStringLiteral("3.1");

		case QAudioFormat::ChannelConfigSurround5Dot0:
			return QStringLiteral("5.0 Surround");

		case QAudioFormat::ChannelConfigSurround5Dot1:
			return QStringLiteral("5.1 Surround");

		case QAudioFormat::ChannelConfigSurround7Dot0:
			return QStringLiteral("7.0 Surround");

		case QAudioFormat::ChannelConfigSurround7Dot1:
			return QStringLiteral("7.1 Surround");

		default:
			return {};
	}
}

auto EnumSerializer::toString(QAudioFormat::SampleFormat sampleFormat) -> QString
{
	switch (sampleFormat)
	{
		case QAudioFormat::Unknown:
			return QStringLiteral("Unknown");

		case QAudioFormat::UInt8:
			return QStringLiteral("8-bit unsigned");

		case QAudioFormat::Int16:
			return QStringLiteral("16-bit signed");

		case QAudioFormat::Int32:
			return QStringLiteral("32-bit signed");

		case QAudioFormat::Float:
			return QStringLiteral("32-bit float");

		default:
			return {};
	}
}

auto EnumSerializer::toString(const QtAudio::State state) -> QString
{
	switch (state)
	{
		case QtAudio::ActiveState:
			return QStringLiteral("Active");

		case QtAudio::SuspendedState:
			return QStringLiteral("Suspended");

		case QtAudio::StoppedState:
			return QStringLiteral("Stopped");

		case QtAudio::IdleState:
			return QStringLiteral("Idle");

		default:
			return {};
	}
}

auto EnumSerializer::toString(const QtAudio::Error error) -> QString
{
	switch (error)
	{
		case QtAudio::NoError:
			return QStringLiteral("No error");

		case QtAudio::OpenError:
			return QStringLiteral("Error occurred opening audio device");

		case QtAudio::IOError:
			return QStringLiteral("Error occurred during read/write of audio device");

		case QtAudio::FatalError:
			return QStringLiteral("Audio device unusable at this time");

		default:
			return {};
	}
}
