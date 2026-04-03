#include "player/metadata.hpp"

#include <QNetworkReply>

MetaData::MetaData()
{
}

MetaData::MetaData(const SongData &songData, const QUrl &url)
{
	mTitle = songData.sngTitle();
	mReleaseDate = songData.digitalReleaseDate().startOfDay();
	mUrl = url;
	mAlbumName = songData.albTitle();

	mArtistNames.reserve(songData.artists().length());
	for (const SongData::Artist &artist: songData.artists())
	{
		mArtistNames.append(artist.artName());
	}

	const QNetworkRequest request(songData.albPictureUrl());
	const QNetworkReply *reply = mHttp.get(request);

	connect(reply, &QNetworkReply::finished,
		this, &MetaData::onAlbumPictureFinished);
}

MetaData::MetaData(const QMediaMetaData &metaData)
{
	mTitle = metaData.value(QMediaMetaData::Title).toString();
	mArtistNames = metaData.value(QMediaMetaData::Author).value<QStringList>();
	mReleaseDate = metaData.value(QMediaMetaData::Date).toDateTime();
	mUrl = metaData.value(QMediaMetaData::Url).toUrl();
	mAlbumName = metaData.value(QMediaMetaData::AlbumTitle).toString();
	mAlbumPicture = metaData.value(QMediaMetaData::CoverArtImage).value<QImage>();
}

auto MetaData::title() const -> const QString &
{
	return mTitle;
}

auto MetaData::artistNames() const -> const QStringList &
{
	return mArtistNames;
}

auto MetaData::releaseDate() const -> const QDateTime &
{
	return mReleaseDate;
}

auto MetaData::url() const -> const QUrl &
{
	return mUrl;
}

auto MetaData::albumName() const -> const QString &
{
	return mAlbumName;
}

auto MetaData::albumPicture() const -> const QImage &
{
	return mAlbumPicture;
}

void MetaData::onAlbumPictureFinished()
{
	auto *reply = qobject_cast<QNetworkReply *>(sender());

	mAlbumPicture = QImage::fromData(reply->readAll(), "jpg");
	reply->deleteLater();

	emit albumPictureChanged();
}
