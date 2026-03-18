#pragma once

#include "deezer/enums/explicitcontent.hpp"

#include <QJsonObject>

class SongData final
{
public:
	[[nodiscard]]
	static auto fromJson(const QJsonObject &json) -> SongData;

	class Artist final
	{
	public:
		static auto fromJson(const QJsonObject &json) -> Artist;

	private:
		Artist() = default;

		qint64 mArtId = 0;
		QString mArtName;
		QString mArtPicture;
	};

	[[nodiscard]]
	auto trackToken() const -> const QString &;

	[[nodiscard]]
	auto trackTokenExpire() const -> const QDateTime &;

private:
	SongData() = default;

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
