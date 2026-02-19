#pragma once

#include <QJsonDocument>
#include <QString>

class UserData final
{
public:
	[[nodiscard]]
	static auto fromJson(const QJsonDocument &json) -> UserData;

	[[nodiscard]]
	auto userId() const -> qint64;

	[[nodiscard]]
	auto blogName() const -> QString;

private:
	UserData() = default;

	qint64 mUserId;
	QString mBlogName;
};
