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

		[[nodiscard]]
		auto id() const -> qint64;

		[[nodiscard]]
		auto name() const -> const QString &;

		[[nodiscard]]
		auto picture() const -> const QUrl &;

		[[nodiscard]]
		auto radio() const -> bool;

		[[nodiscard]]
		auto role() const -> const QString &;

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

		[[nodiscard]]
		auto id() const -> qint64;

		[[nodiscard]]
		auto name() const -> const QString &;

		[[nodiscard]]
		auto picture() const -> const QUrl &;

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

		[[nodiscard]]
		auto id() const -> qint64;

		[[nodiscard]]
		auto readable() const -> bool;

		[[nodiscard]]
		auto title() const -> const QString &;

		[[nodiscard]]
		auto titleShort() const -> const QString &;

		[[nodiscard]]
		auto duration() const -> qint32;

		[[nodiscard]]
		auto rank() const -> qint64;

		[[nodiscard]]
		auto explicitLyrics() const -> bool;

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

	[[nodiscard]]
	auto cover() const -> const QUrl &;

	[[nodiscard]]
	auto genres() const -> const Page<Genre> &;

	[[nodiscard]]
	auto label() const -> const QString &;

	[[nodiscard]]
	auto nbTracks() const -> qint32;

	[[nodiscard]]
	auto duration() const -> qint64;

	[[nodiscard]]
	auto fans() const -> qint64;

	[[nodiscard]]
	auto releaseDate() const -> const QDate &;

	[[nodiscard]]
	auto recordType() const -> const QString &;

	[[nodiscard]]
	auto available() const -> bool;

	[[nodiscard]]
	auto explicitLyrics() const -> bool;

	[[nodiscard]]
	auto explicitContentLyrics() const -> ExplicitContent;

	[[nodiscard]]
	auto explicitContentCover() const -> ExplicitContent;

	[[nodiscard]]
	auto contributors() const -> const QList<Contributor> &;

	[[nodiscard]]
	auto artist() const -> const Artist &;

	[[nodiscard]]
	auto tracks() const -> const Page<Track> &;

private:
	Album() = default;

	qint64 mId;
	QString mTitle;
	QUrl mCover;
	Page<Genre> mGenres;
	QString mLabel;
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
	Page<Track> mTracks;
};
