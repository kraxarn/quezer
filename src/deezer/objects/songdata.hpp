#pragma once

#include "deezer/enums/explicitcontent.hpp"

#include <QJsonObject>

class SongData final
{
public:
	SongData() = default;

	[[nodiscard]]
	static auto fromJson(const QJsonObject &json) -> SongData;

	class Artist final
	{
	public:
		static auto fromJson(const QJsonObject &json) -> Artist;

		[[nodiscard]]
		auto artName() const -> const QString &;

	private:
		Artist() = default;

		qint64 mArtId = 0;
		QString mArtName;
		QString mArtPicture;
	};

	class Contributors final
	{
	public:
		static auto fromJson(const QJsonObject &json) -> Contributors;

	private:
		QStringList mMainArtist;
		QStringList mArtist;
		QStringList mComposer;
		QStringList mAuthor;
	};

	[[nodiscard]]
	auto albPicture() const -> const QString &;

	[[nodiscard]]
	auto albPictureUrl() const -> QUrl;

	[[nodiscard]]
	auto albTitle() const -> const QString &;

	[[nodiscard]]
	auto artists() const -> const QList<Artist> &;

	[[nodiscard]]
	auto digitalReleaseDate() const -> const QDate &;

	[[nodiscard]]
	auto sngId() const -> qint64;

	[[nodiscard]]
	auto sngTitle() const -> const QString &;

	[[nodiscard]]
	auto trackToken() const -> const QString &;

	[[nodiscard]]
	auto trackTokenExpire() const -> const QDateTime &;

private:
	qint64 mAlbId = 0;
	QString mAlbPicture;
	QString mAlbTitle;
	QList<Artist> mArtists;
	QDate mDigitalReleaseDate;
	qint64 mDuration = 0;
	ExplicitContent mExplicitLyrics = ExplicitContent::NotExplicit;
	QDate mPhysicalReleaseDate;
	qint64 mSngId = 0;
	QString mSngTitle;
	QString mTrackToken;
	QDateTime mTrackTokenExpire;
};
