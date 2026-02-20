#include "deezer/objects/userdata.hpp"

#include <QJsonObject>

auto UserData::fromJson(const QJsonObject &json) -> UserData
{
	UserData result;

	const QJsonObject results = json
		.value(QStringLiteral("results")).toObject();

	const QJsonObject user = results
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

	result.mCheckForm = results.value(QStringLiteral("checkForm")).toString();

	return result;
}

auto UserData::userId() const -> qint64
{
	return mUserId;
}

auto UserData::blogName() const -> const QString &
{
	return mBlogName;
}

auto UserData::checkForm() const -> const QString &
{
	return mCheckForm;
}
