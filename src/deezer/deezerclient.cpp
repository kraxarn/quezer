#include "deezer/deezerclient.hpp"
#include "deezer/objects/page.hpp"
#include "deezer/objects/searchalbum.hpp"

#include <QCoreApplication>
#include <QHttpHeaders>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QUrlQuery>

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

	ApiResponse *response = mGw->userData();
	connect(response, &ApiResponse::finished, [response]() -> void
	{
		if (!response->isValid())
		{
			qWarning() << "Request failed:" << response->errorString();
			return;
		}

		const UserData &userData = response->value<UserData>();
		qDebug().nospace() << "Welcome " << userData.blogName() << "!";
	});

	ApiResponse *searchResponse = mApi->search(SearchMediaType::Album,
		QStringLiteral("Penny's Big Breakaway"));

	connect(searchResponse, &ApiResponse::finished, [searchResponse]
	{
		if (!searchResponse->isValid())
		{
			qWarning() << "Request failed:" << searchResponse->errorString();
			return;
		}

		const Page<SearchAlbum> page = searchResponse->value<Page<SearchAlbum>>();
		qDebug() << "Results:" << page.total();
		qDebug() << "Has next:" << page.next().isValid();

		for (const SearchAlbum &album: page.data())
		{
			qInfo() << "Album:" << album.title() << "by" << album.artist().name();
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
