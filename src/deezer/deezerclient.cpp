#include "deezer/deezerclient.hpp"
#include "deezer/objects/page.hpp"
#include "deezer/objects/searchalbum.hpp"

#include <QCoreApplication>
#include <QHttpHeaders>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QUrlQuery>

#include "objects/album.hpp"

DeezerClient::DeezerClient(QObject *parent)
	: QObject(parent),
	mHttp(new QNetworkAccessManager(this)),
	mGw(new DeezerGw(mHttp, this)),
	mApi(new DeezerApi(mHttp, this))
{
}

auto DeezerClient::login(const QString &arl) -> bool
{
	QNetworkCookieJar *cookies = mHttp->cookieJar();

	QNetworkCookie cookie;
	cookie.setDomain(QStringLiteral(".deezer.com"));
	cookie.setName(QStringLiteral("arl").toUtf8());
	cookie.setValue(arl.toUtf8());
	cookie.setPath(QStringLiteral("/"));
	cookie.setHttpOnly(true);

	if (!cookies->insertCookie(cookie))
	{
		return false;
	}

	ApiResponse *userResponse = mGw->userData();
	connect(userResponse, &ApiResponse::finished, [userResponse]() -> void
	{
		if (!userResponse->isValid())
		{
			qWarning() << "Request failed:" << userResponse->errorString();
			userResponse->deleteLater();
			return;
		}

		const UserData &userData = userResponse->value<UserData>();
		userResponse->deleteLater();

		qDebug().nospace() << "Welcome " << userData.blogName() << "!";
	});

	ApiResponse *searchResponse = mApi->search(SearchMediaType::Album,
		QStringLiteral("Penny's Big Breakaway"));

	connect(searchResponse, &ApiResponse::finished, [this, searchResponse]() -> void
	{
		if (!searchResponse->isValid())
		{
			qWarning() << "Request failed:" << searchResponse->errorString();
			searchResponse->deleteLater();
			return;
		}

		const auto page = searchResponse->value<Page<SearchAlbum>>();
		searchResponse->deleteLater();

		qDebug() << "Results:" << page.total();
		qDebug() << "Has next:" << page.next().isValid();

		for (const SearchAlbum &searchAlbum: page.data())
		{
			qInfo()
				<< "Album:" << searchAlbum.title()
				<< "by" << searchAlbum.artist().name();

			ApiResponse *albumResponse = mApi->album(searchAlbum.id());
			connect(albumResponse, &ApiResponse::finished, [this, albumResponse]() -> void
			{
				if (!albumResponse->isValid())
				{
					qWarning() << "Request failed:" << albumResponse->errorString();
					albumResponse->deleteLater();
					return;
				}

				const Album album = albumResponse->value<Album>();

				qInfo() << "Album:" << album.id() << album.title();
			});
		}
	});

	return true;
}

auto DeezerClient::request(const QUrl &url) const -> QNetworkRequest
{
	QNetworkRequest request(url);
	request.setHeaders(headers());
	return request;
}

auto DeezerClient::headers() const -> QHttpHeaders
{
	QHttpHeaders headers;

	headers.append(
		QHttpHeaders::WellKnownHeader::UserAgent,
		QStringLiteral(
			"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
		)
	);

	return headers;
}
