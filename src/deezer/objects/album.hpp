#pragma once

#include "deezer/enums/explicitcontent.hpp"
#include "deezer/objects/genre.hpp"
#include "deezer/objects/page.hpp"

#include <QString>
#include <QUrl>

class Album final
{
public:
	[[nodiscard]]
	static auto fromJson(const QJsonObject &json) -> Album;

	class Contributor
	{
	public:
		[[nodiscard]]
		static auto fromJson(const QJsonObject &json) -> Contributor;

	private:
		Contributor() = default;

		qint64 mId;
		QString mName;
		QUrl mPicture;
		bool mRadio;
		QString mRole;

		friend class Album;
	};

	class Artist
	{
	public:
		[[nodiscard]]
		static auto fromJson(const QJsonObject &json) -> Artist;

	private:
		Artist() = default;

		qint64 mId;
		QString mName;
		QUrl mPicture;

		friend class Album;
	};

	class Track
	{
	public:
		[[nodiscard]]
		static auto fromJson(const QJsonObject &json) -> Track;

	private:
		Track() = default;

		qint64 mId;
		bool mReadable;
		QString mTitle;
		QString mTitleShort;
		qint32 mDuration;
		qint64 mRank;
		bool mExplicitLyrics;

		friend class Album;
	};

	[[nodiscard]]
	auto id() const -> qint64;

	[[nodiscard]]
	auto title() const -> const QString &;

private:
	Album() = default;

	qint64 mId;
	QString mTitle;
	QUrl mCover;
	Page<Genre> mGenres;
	QString mLabel;
	QString mProvider;
	qint32 mNbTracks;
	qint64 mDuration;
	qint64 mFans;
	QDate mReleaseDate;
	QString mRecordType;
	bool mAvailable;
	bool mExplicitLyrics;
	ExplicitContent mExplicitContentLyrics;
	ExplicitContent mExplicitContentCover;
	QList<Contributor> mContributors;
	Artist mArtist;
	QList<Track> mTracks;
};
