#pragma once

#include "deezer/objects/songdata.hpp"

#include <QImage>
#include <QMediaMetaData>
#include <QNetworkAccessManager>
#include <qqmlintegration.h>

class MetaData : public QObject
{
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(QString title READ title NOTIFY titleChanged)
	Q_PROPERTY(QStringList artistNames READ artistNames NOTIFY artistNamesChanged)
	Q_PROPERTY(QDateTime releaseDate READ releaseDate NOTIFY releaseDateChanged)
	Q_PROPERTY(QUrl url READ url NOTIFY urlChanged)
	Q_PROPERTY(QString albumName READ albumName NOTIFY albumNameChanged)
	Q_PROPERTY(QImage albumPicture READ albumPicture NOTIFY albumPictureChanged)

public:
	MetaData();

	MetaData(const SongData &songData, const QUrl &url);

	explicit MetaData(const QMediaMetaData &metaData);

	[[nodiscard]]
	auto title() const -> const QString &;

	[[nodiscard]]
	auto artistNames() const -> const QStringList &;

	[[nodiscard]]
	auto releaseDate() const -> const QDateTime &;

	[[nodiscard]]
	auto url() const -> const QUrl &;

	[[nodiscard]]
	auto albumName() const -> const QString &;

	[[nodiscard]]
	auto albumPicture() const -> const QImage &;

signals:
	void titleChanged();

	void artistNamesChanged();

	void releaseDateChanged();

	void urlChanged();

	void albumNameChanged();

	void albumPictureChanged();

private:
	QNetworkAccessManager mHttp;

	QString mTitle;
	QStringList mArtistNames;
	QDateTime mReleaseDate;
	QUrl mUrl;
	QString mAlbumName;
	QImage mAlbumPicture;

	void onAlbumPictureFinished();
};
