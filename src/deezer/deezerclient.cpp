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

auto DeezerClient::login(const QString &arl) const -> bool
{
	if (arl.isEmpty())
	{
		return false;
	}

	QNetworkCookieJar *cookies = mHttp->cookieJar();

	QNetworkCookie cookie;
	cookie.setDomain(QStringLiteral(".deezer.com"));
	cookie.setName(QStringLiteral("arl").toUtf8());
	cookie.setValue(arl.toUtf8());
	cookie.setPath(QStringLiteral("/"));
	cookie.setHttpOnly(true);

	return cookies->insertCookie(cookie);
}

auto DeezerClient::userData() const -> ApiResponse *
{
	return mGw->userData();
}

auto DeezerClient::search(const SearchMediaType mediaType, const QString &query,
	const SearchMode mode, const SearchOrder order) const -> ApiResponse *
{
	return mApi->search(mediaType, query, mode, order);
}

auto DeezerClient::album(const qint64 albumId) const -> ApiResponse *
{
	return mApi->album(albumId);
}

auto DeezerClient::gw() const -> DeezerGw &
{
	return *mGw;
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
