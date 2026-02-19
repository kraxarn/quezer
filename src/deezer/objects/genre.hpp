#pragma once

#include <QJsonObject>
#include <QString>
#include <QUrl>

class Genre final
{
public:
	[[nodiscard]]
	static auto fromJson(const QJsonObject &json) -> Genre;

	[[nodiscard]]
	auto id() const -> qint64;

	[[nodiscard]]
	auto name() const -> const QString &;

	[[nodiscard]]
	auto picture() const -> const QUrl &;

private:
	Genre() = default;

	qint64 mId;
	QString mName;
	QUrl mPicture;
};
