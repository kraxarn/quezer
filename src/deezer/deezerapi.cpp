#include "deezer/deezerapi.hpp"

#include <QHttpHeaders>
#include <QIODevice>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QUrlQuery>

DeezerApi::DeezerApi(QObject *parent)
	: QObject(parent),
	mHttp(this)
{
}

auto DeezerApi::login(const QString &arl) -> bool
{
	QNetworkCookieJar *cookies = mHttp.cookieJar();

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

	QNetworkReply *reply = gwApiCall(QStringLiteral("deezer.getUserData"));
	connect(reply, &QNetworkReply::finished, [reply]() -> void
	{
		if (reply->error() != QNetworkReply::NoError)
		{
			qWarning() << "Request failed:" << reply->errorString();
		}

		const QByteArray response = reply->readAll();
		reply->deleteLater();
		const QJsonDocument json = QJsonDocument::fromJson(response);
		qDebug().nospace() << "Welcome " << json.object()
			.value(QStringLiteral("results")).toObject()
			.value(QStringLiteral("USER")).toObject()
			.value(QStringLiteral("BLOG_NAME")).toString()
			<< "!";
	});

	return true;
}

auto DeezerApi::gwApiCall(const QString &method) -> QNetworkReply *
{
	QUrl url(QStringLiteral("https://www.deezer.com/ajax/gw-light.php"));
	url.setQuery({
		{
			{QStringLiteral("api_version"), QStringLiteral("1.0")},
			{QStringLiteral("api_token"), QStringLiteral("null")},
			{QStringLiteral("input"), QStringLiteral("3")},
			{QStringLiteral("method"), method},
		},
	});

	return mHttp.post(request(url), QStringLiteral("{}").toUtf8());
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
