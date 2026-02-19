#include "deezer/api/deezerapi.hpp"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrlQuery>

DeezerApi::DeezerApi(QNetworkAccessManager *http, QObject *parent)
	: QObject(parent),
	mHttp(http)
{
}

auto DeezerApi::search(const SearchMediaType mediaType, const QString &query,
	const SearchMode mode, const SearchOrder order) -> ApiResponse *
{
	const QString path = QStringLiteral("search/%1")
		.arg(searchMediaTypeString(mediaType));

	const QUrlQuery args({
		{QStringLiteral("q"), query},
		{QStringLiteral("strict"), searchModeString(mode)},
		{QStringLiteral("order"), searchOrderString(order)},
	});

	QNetworkReply *reply = call(path, args);
	return new ApiResponse(reply, this);
}

auto DeezerApi::call(const QString &path, const QUrlQuery &args) const -> QNetworkReply *
{
	QUrl url(QStringLiteral("https://api.deezer.com/%1").arg(path));
	url.setQuery(args);

	const QNetworkRequest request(url);
	return mHttp->get(request);
}

auto DeezerApi::searchMediaTypeString(const SearchMediaType mediaType) -> QString
{
	switch (mediaType)
	{
		case SearchMediaType::Album:
			return QStringLiteral("album");

		case SearchMediaType::Artist:
			return QStringLiteral("artist");

		case SearchMediaType::Playlist:
			return QStringLiteral("playlist");

		case SearchMediaType::Radio:
			return QStringLiteral("radio");

		case SearchMediaType::Track:
			return QStringLiteral("track");

		case SearchMediaType::User:
			return QStringLiteral("user");

		default:
			return QString();
	}
}

auto DeezerApi::searchModeString(const SearchMode mode) -> QString
{
	switch (mode)
	{
		case SearchMode::Fuzzy:
			return QStringLiteral("off");

		case SearchMode::Strict:
			return QStringLiteral("on");

		default:
			return QString();
	}
}

auto DeezerApi::searchOrderString(const SearchOrder order) -> QString
{
	switch (order)
	{
		case SearchOrder::Ranking:
			return QStringLiteral("RANKING");

		case SearchOrder::TrackAsc:
			return QStringLiteral("TRACK_ASC");

		case SearchOrder::TrackDesc:
			return QStringLiteral("TRACK_DESC");

		case SearchOrder::ArtistAsc:
			return QStringLiteral("ARTIST_ASC");

		case SearchOrder::ArtistDesc:
			return QStringLiteral("ARTIST_DESC");

		case SearchOrder::AlbumAsc:
			return QStringLiteral("ALBUM_ASC");

		case SearchOrder::AlbumDesc:
			return QStringLiteral("ALBUM_DESC");

		case SearchOrder::RatingAsc:
			return QStringLiteral("RATING_ASC");

		case SearchOrder::RatingDesc:
			return QStringLiteral("RATING_DESC");

		case SearchOrder::DurationAsc:
			return QStringLiteral("DURATION_ASC");

		case SearchOrder::DurationDesc:
			return QStringLiteral("DURATION_DESC");

		default:
			return QString();
	}
}
