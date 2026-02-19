#include "deezer/userdata.hpp"

#include <QJsonObject>

auto UserData::fromJson(const QJsonDocument &json) -> UserData
{
	UserData result;

	const QJsonObject user = json.object()
		.value(QStringLiteral("results")).toObject()
		.value(QStringLiteral("USER")).toObject();

	result.mUserId = user.value(QStringLiteral("USER_ID")).toInteger();
	result.mBlogName = user.value(QStringLiteral("BLOG_NAME")).toString();

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
