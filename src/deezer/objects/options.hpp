#pragma once

#include <QJsonObject>

class Options final
{
public:
	[[nodiscard]]
	static auto fromJson(const QJsonObject &json) -> Options;

	[[nodiscard]]
	auto streaming() const -> bool;

	[[nodiscard]]
	auto offline() const -> bool;

	[[nodiscard]]
	auto hq() const -> bool;

	[[nodiscard]]
	auto adsDisplay() const -> bool;

	[[nodiscard]]
	auto lossless() const -> bool;

	[[nodiscard]]
	auto radio() const -> bool;

private:
	Options() = default;

	bool mStreaming;
	bool mOffline;
	bool mHq;
	bool mAdsDisplay;
	bool mLossless;
	bool mRadio;
};
