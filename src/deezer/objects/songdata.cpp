#include "deezer/objects/songdata.hpp"

#include <QJsonArray>

auto SongData::fromJson(const QJsonObject &json) -> SongData
{
	SongData songData;

	const QJsonObject results = json
		.value(QStringLiteral("results")).toObject();

	const QJsonArray artists = results
		.value(QStringLiteral("ARTISTS")).toArray();

	songData.mAlbId = results.value(QStringLiteral("ALB_ID")).toString().toLongLong();
	songData.mAlbPicture = results.value(QStringLiteral("ALB_PICTURE")).toString();
	songData.mAlbTitle = results.value(QStringLiteral("ALB_TITLE")).toString();

	songData.mArtists.reserve(artists.size());
	for (const QJsonValueConstRef artist: artists)
	{
		songData.mArtists.append(Artist::fromJson(artist.toObject()));
	}

	songData.mDigitalReleaseDate = QDate::fromString(
		results.value(QStringLiteral("DIGITAL_RELEASE_DATE")).toString(),
		Qt::ISODate
	);
	songData.mDuration = results.value(QStringLiteral("DURATION")).toString().toLongLong();
	songData.mExplicitLyrics = static_cast<ExplicitContent>(
		results.value(QStringLiteral("EXPLICIT_LYRICS")).toString().toInt()
	);
	songData.mPhysicalReleaseDate = QDate::fromString(
		results.value(QStringLiteral("PHYSICAL_RELEASE_DATE")).toString(),
		Qt::ISODate
	);
	songData.mSngId = results.value(QStringLiteral("SNG_ID")).toString().toLongLong();
	songData.mSngTitle = results.value(QStringLiteral("SNG_TITLE")).toString();

	songData.mTrackToken = results.value(QStringLiteral("TRACK_TOKEN")).toString();
	songData.mTrackTokenExpire = QDateTime::fromSecsSinceEpoch(
		results.value(QStringLiteral("TRACK_TOKEN_EXPIRE")).toInteger()
	);

	return songData;
}

auto SongData::Artist::fromJson(const QJsonObject &json) -> Artist
{
	Artist artist;

	artist.mArtId = json.value(QStringLiteral("ART_ID")).toString().toLongLong();
	artist.mArtName = json.value(QStringLiteral("ART_NAME")).toString();
	artist.mArtPicture = json.value(QStringLiteral("ART_PICTURE")).toString();

	return artist;
}

auto SongData::sngId() const -> qint64
{
	return mSngId;
}

auto SongData::trackToken() const -> const QString &
{
	return mTrackToken;
}

auto SongData::trackTokenExpire() const -> const QDateTime &
{
	return mTrackTokenExpire;
}
