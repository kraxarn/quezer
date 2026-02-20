#include "deezer/objects/album.hpp"

auto Album::fromJson(const QJsonObject &json) -> Album
{
	Album result;

	const QJsonArray contributors = json
		.value(QStringLiteral("contributors")).toArray();

	const QJsonArray tracks = json
		.value(QStringLiteral("tracks")).toArray();

	result.mId = json.value(QStringLiteral("id")).toInteger();
	result.mTitle = json.value(QStringLiteral("title")).toString();
	result.mCover = json.value(QStringLiteral("cover")).toString();
	result.mGenres = Page<Genre>::fromJson(
		json.value(QStringLiteral("genres")).toObject()
	);
	result.mLabel = json.value(QStringLiteral("label")).toString();
	result.mNbTracks = json.value(QStringLiteral("nb_tracks")).toInt();
	result.mDuration = json.value(QStringLiteral("duration")).toInteger();
	result.mFans = json.value(QStringLiteral("fans")).toInteger();
	result.mReleaseDate = QDate::fromString(
		json.value(QStringLiteral("release_date")).toString(),
		Qt::ISODate
	);
	result.mRecordType = json.value(QStringLiteral("record_type")).toString();
	result.mAvailable = json.value(QStringLiteral("available")).toBool();
	result.mExplicitLyrics = json.value(QStringLiteral("explicit_lyrics")).toBool();
	result.mExplicitContentLyrics = static_cast<ExplicitContent>(
		json.value(QStringLiteral("explicit_content_lyrics")).toInt()
	);
	result.mExplicitContentCover = static_cast<ExplicitContent>(
		json.value(QStringLiteral("explicit_content_cover")).toInt()
	);

	result.mContributors.reserve(contributors.size());
	for (const QJsonValueConstRef contributor: contributors)
	{
		result.mContributors.append(Contributor::fromJson(contributor.toObject()));
	}

	result.mArtist = Artist::fromJson(
		json.value(QStringLiteral("artist")).toObject()
	);

	result.mTracks.reserve(tracks.size());
	for (const QJsonValueConstRef track: tracks)
	{
		result.mTracks.append(Track::fromJson(track.toObject()));
	}

	return result;
}

auto Album::Contributor::fromJson(const QJsonObject &json) -> Contributor
{
	Contributor result;

	result.mId = json.value(QStringLiteral("id")).toInteger();
	result.mName = json.value(QStringLiteral("name")).toString();
	result.mPicture = json.value(QStringLiteral("picture")).toString();
	result.mRadio = json.value(QStringLiteral("radio")).toBool();
	result.mRole = json.value(QStringLiteral("role")).toString();

	return result;
}

auto Album::Contributor::id() const -> qint64
{
	return mId;
}

auto Album::Contributor::name() const -> const QString &
{
	return mName;
}

auto Album::Contributor::picture() const -> const QUrl &
{
	return mPicture;
}

auto Album::Contributor::radio() const -> bool
{
	return mRadio;
}

auto Album::Contributor::role() const -> const QString &
{
	return mRole;
}

auto Album::Artist::fromJson(const QJsonObject &json) -> Artist
{
	Artist result;

	result.mId = json.value(QStringLiteral("id")).toInteger();
	result.mName = json.value(QStringLiteral("name")).toString();
	result.mPicture = json.value(QStringLiteral("picture")).toString();

	return result;
}

auto Album::Artist::id() const -> qint64
{
	return mId;
}

auto Album::Artist::name() const -> const QString &
{
	return mName;
}

auto Album::Artist::picture() const -> const QUrl &
{
	return mPicture;
}

auto Album::Track::fromJson(const QJsonObject &json) -> Track
{
	Track result;

	result.mId = json.value(QStringLiteral("id")).toInteger();
	result.mReadable = json.value(QStringLiteral("readable")).toBool();
	result.mTitle = json.value(QStringLiteral("title")).toString();
	result.mTitleShort = json.value(QStringLiteral("title_short")).toString();
	result.mDuration = json.value(QStringLiteral("duration")).toInt();
	result.mRank = json.value(QStringLiteral("rank")).toInteger();
	result.mExplicitLyrics = json.value(QStringLiteral("explicit_lyrics")).toBool();

	return result;
}

auto Album::Track::id() const -> qint64
{
	return mId;
}

auto Album::Track::readable() const -> bool
{
	return mReadable;
}

auto Album::Track::title() const -> const QString &
{
	return mTitle;
}

auto Album::Track::titleShort() const -> const QString &
{
	return mTitleShort;
}

auto Album::Track::duration() const -> qint32
{
	return mDuration;
}

auto Album::Track::rank() const -> qint64
{
	return mRank;
}

auto Album::Track::explicitLyrics() const -> bool
{
	return mExplicitLyrics;
}

auto Album::id() const -> qint64
{
	return mId;
}

auto Album::title() const -> const QString &
{
	return mTitle;
}

auto Album::cover() const -> const QUrl &
{
	return mCover;
}

auto Album::genres() const -> const Page<Genre> &
{
	return mGenres;
}

auto Album::label() const -> const QString &
{
	return mLabel;
}

auto Album::nbTracks() const -> qint32
{
	return mNbTracks;
}

auto Album::duration() const -> qint64
{
	return mDuration;
}

auto Album::fans() const -> qint64
{
	return mFans;
}

auto Album::releaseDate() const -> const QDate &
{
	return mReleaseDate;
}

auto Album::recordType() const -> const QString &
{
	return mRecordType;
}

auto Album::available() const -> bool
{
	return mAvailable;
}

auto Album::explicitLyrics() const -> bool
{
	return mExplicitLyrics;
}

auto Album::explicitContentLyrics() const -> ExplicitContent
{
	return mExplicitContentLyrics;
}

auto Album::explicitContentCover() const -> ExplicitContent
{
	return mExplicitContentCover;
}

auto Album::contributors() const -> const QList<Contributor> &
{
	return mContributors;
}

auto Album::artist() const -> const Artist &
{
	return mArtist;
}

auto Album::tracks() const -> const QList<Track> &
{
	return mTracks;
}
