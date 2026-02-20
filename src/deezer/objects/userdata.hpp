#pragma once

#include <QJsonObject>
#include <QString>

class UserData final
{
public:
	[[nodiscard]]
	static auto fromJson(const QJsonObject &json) -> UserData;

	[[nodiscard]]
	auto userId() const -> qint64;

	[[nodiscard]]
	auto blogName() const -> const QString &;

	[[nodiscard]]
	auto licenseToken() const -> const QString &;

	[[nodiscard]]
	auto checkForm() const -> const QString &;

private:
	UserData() = default;

	qint64 mUserId;
	QString mBlogName;
	QString mUserPicture;
	QString mLicenseToken;
	bool mCanStreamHq;
	bool mCanStreamLossless;
	QString mCountry;
	QString mLanguage;
	QString mLovedTracks;
	QString mCheckForm;
};
