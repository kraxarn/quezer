#include "deezer/objects/options.hpp"

auto Options::fromJson(const QJsonObject &json) -> Options
{
	Options result{};

	result.mStreaming = json.value(QStringLiteral("streaming")).toBool();
	result.mOffline = json.value(QStringLiteral("offline")).toBool();
	result.mHq = json.value(QStringLiteral("hq")).toBool();
	result.mAdsDisplay = json.value(QStringLiteral("ads_display")).toBool();
	result.mLossless = json.value(QStringLiteral("lossless")).toBool();
	result.mRadio = json.value(QStringLiteral("radio")).toBool();

	return result;
}

auto Options::streaming() const -> bool
{
	return mStreaming;
}

auto Options::offline() const -> bool
{
	return mOffline;
}

auto Options::hq() const -> bool
{
	return mHq;
}

auto Options::adsDisplay() const -> bool
{
	return mAdsDisplay;
}

auto Options::lossless() const -> bool
{
	return mLossless;
}

auto Options::radio() const -> bool
{
	return mRadio;
}
