#include "deezer/deezerapi.hpp"

#include <QCoreApplication>
#include <QHttpHeaders>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QUrlQuery>

DeezerApi::DeezerApi(QObject *parent)
	: QObject(parent),
	mHttp(new QNetworkAccessManager(this)),
	mGwApi(new GwApi(mHttp, this))
{
}

auto DeezerApi::login(const QString &arl) -> bool
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

	ApiResponse *response = mGwApi->userData();
	connect(response, &ApiResponse::finished, [response]() -> void
	{
		if (!response->isValid())
		{
			qWarning() << "Request failed:" << response->errorString();
			return;
		}

		const UserData &userData = response->value<UserData>();
		qDebug().nospace() << "Welcome " << userData.blogName() << "!";

		QCoreApplication::exit(0);
	});

	return true;
}

auto DeezerApi::request(const QUrl &url) const -> QNetworkRequest
{
	QNetworkRequest request(url);
	request.setHeaders(headers());
	return request;
}

auto DeezerApi::headers() const -> QHttpHeaders
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
