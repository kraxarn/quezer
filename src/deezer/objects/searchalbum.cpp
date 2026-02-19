#include "deezer/objects/searchalbum.hpp"

#include <QJsonObject>

auto SearchAlbum::fromJson(const QJsonObject &json) -> SearchAlbum
{
	SearchAlbum result;

	const QJsonObject artist = json
		.value(QStringLiteral("artist")).toObject();

	result.mId = json.value(QStringLiteral("id")).toInt();
	result.mTitle = json.value(QStringLiteral("title")).toString();
	result.mCover = json.value(QStringLiteral("cover")).toString();
	result.mNbTracks = json.value(QStringLiteral("nb_tracks")).toInt();
	result.mRecordType = json.value(QStringLiteral("record_type")).toString();
	result.mExplicitLyrics = json.value(QStringLiteral("explicit_lyrics")).toBool();

	result.mArtist.mId = artist.value(QStringLiteral("id")).toInt();
	result.mArtist.mName = artist.value(QStringLiteral("name")).toString();
	result.mArtist.mPicture = artist.value(QStringLiteral("picture")).toString();

	return result;
}

auto SearchAlbum::Artist::id() const -> qint64
{
	return mId;
}

auto SearchAlbum::Artist::name() const -> const QString &
{
	return mName;
}

auto SearchAlbum::Artist::picture() const -> const QUrl &
{
	return mPicture;
}

auto SearchAlbum::id() const -> qint64
{
	return mId;
}

auto SearchAlbum::title() const -> const QString &
{
	return mTitle;
}

auto SearchAlbum::cover() const -> const QUrl &
{
	return mCover;
}

auto SearchAlbum::nbTracks() const -> qint32
{
	return mNbTracks;
}

auto SearchAlbum::recordType() const -> const QString &
{
	return mRecordType;
}

auto SearchAlbum::explicitLyrics() const -> bool
{
	return mExplicitLyrics;
}

auto SearchAlbum::artist() const -> const Artist &
{
	return mArtist;
}
