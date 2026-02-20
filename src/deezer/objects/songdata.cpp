#include "deezer/objects/songdata.hpp"

auto SongData::fromJson(const QJsonObject &json) -> SongData
{
	SongData result;

	const QJsonObject results = json
		.value(QStringLiteral("results")).toObject();

	result.mTrackToken = results.value(QStringLiteral("TRACK_TOKEN")).toString();
	result.mTrackTokenExpire = QDateTime::fromSecsSinceEpoch(
		results.value(QStringLiteral("TRACK_TOKEN_EXPIRE")).toInteger()
	);

	return result;
}

auto SongData::trackToken() const -> const QString &
{
	return mTrackToken;
}

auto SongData::trackTokenExpire() const -> const QDateTime &
{
	return mTrackTokenExpire;
}
