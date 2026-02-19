#pragma once

#include <QJsonDocument>
#include <QUrl>

class SearchAlbum final
{
public:
	[[nodiscard]]
	static auto fromJson(const QJsonDocument &json) -> SearchAlbum;

	class Artist final
	{
	public:
		[[nodiscard]]
		auto id() const -> qint64;

		[[nodiscard]]
		auto name() const -> const QString &;

		[[nodiscard]]
		auto picture() const -> const QUrl &;

	private:
		qint64 mId;
		QString mName;
		QUrl mPicture;

		friend class SearchAlbum;
	};

	[[nodiscard]]
	auto id() const -> qint64;

	[[nodiscard]]
	auto title() const -> const QString &;

	[[nodiscard]]
	auto cover() const -> const QUrl &;

	[[nodiscard]]
	auto nbTracks() const -> qint32;

	[[nodiscard]]
	auto recordType() const -> const QString &;

	[[nodiscard]]
	auto explicitLyrics() const -> bool;

	[[nodiscard]]
	auto artist() const -> const Artist &;

private:
	SearchAlbum() = default;

	qint64 mId;
	QString mTitle;
	QUrl mCover;
	qint32 mNbTracks;
	QString mRecordType;
	bool mExplicitLyrics;
	Artist mArtist;
};
