#pragma once

#include <QJsonObject>

class SongData final
{
public:
	[[nodiscard]]
	static auto fromJson(const QJsonObject &json) -> SongData;

	[[nodiscard]]
	auto trackToken() const -> const QString &;

	[[nodiscard]]
	auto trackTokenExpire() const -> const QDateTime &;

private:
	SongData() = default;

	QString mTrackToken;
	QDateTime mTrackTokenExpire;
};
