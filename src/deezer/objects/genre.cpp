#include "deezer/objects/genre.hpp"

auto Genre::fromJson(const QJsonObject &json) -> Genre
{
	Genre result;

	result.mId = json.value(QStringLiteral("id")).toInteger();
	result.mName = json.value(QStringLiteral("name")).toString();
	result.mPicture = json.value(QStringLiteral("picture")).toString();

	return result;
}

auto Genre::id() const -> qint64
{
	return mId;
}

auto Genre::name() const -> const QString &
{
	return mName;
}

auto Genre::picture() const -> const QUrl &
{
	return mPicture;
}
