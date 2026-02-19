#include "deezer/userdata.hpp"

#include <QJsonObject>

auto UserData::fromJson(const QJsonDocument &json) -> UserData
{
	UserData result;

	const QJsonObject user = json.object()
		.value(QStringLiteral("results")).toObject()
		.value(QStringLiteral("USER")).toObject();

	const QJsonObject options = user
		.value(QStringLiteral("OPTIONS")).toObject();

	const QJsonObject settingGlobal = user
		.value(QStringLiteral("SETTING")).toObject()
		.value(QStringLiteral("global")).toObject();

	result.mUserId = user.value(QStringLiteral("USER_ID")).toInteger();
	result.mBlogName = user.value(QStringLiteral("BLOG_NAME")).toString();
	result.mUserPicture = user.value(QStringLiteral("USER_PICTURE")).toString();
	result.mLicenseToken = options.value(QStringLiteral("license_token")).toString();
	result.mCountry = options.value(QStringLiteral("license_country")).toString();
	result.mLanguage = settingGlobal.value(QStringLiteral("language")).toString();
	result.mLovedTracks = user.value(QStringLiteral("LOVEDTRACKS_ID")).toString();

	result.mCanStreamHq = options.value(QStringLiteral("web_hq")).toBool()
		|| options.value(QStringLiteral("mobile_hq")).toBool();

	result.mCanStreamLossless = options.value(QStringLiteral("web_lossless")).toBool()
		|| options.value(QStringLiteral("mobile_lossless")).toBool();

	return result;
}

auto UserData::userId() const -> qint64
{
	return mUserId;
}

auto UserData::blogName() const -> QString
{
	return mBlogName;
}
