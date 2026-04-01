#include "deezer/deezerclient.hpp"
#include "settings.hpp"
#include "deezer/apipage.hpp"
#include "deezer/objects/album.hpp"

#include <QCoreApplication>
#include <QHttpHeaders>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QUrlQuery>

DeezerClient *DeezerClient::sInstance = nullptr;

DeezerClient::DeezerClient(QObject *parent)
	: QObject(parent),
	mHttp(new QNetworkAccessManager(this)),
	mGw(mHttp, this),
	mApi(mHttp, this),
	mMedia(mHttp, this)
{
	Settings settings;
	const QString arl = settings.arl();
	const QDateTime expiration = settings.arlExpiration();

	if (!arl.isEmpty() && expiration.isValid())
	{
		if (!login(arl, expiration))
		{
			settings.removeArl();
			settings.removeArlExpiration();
		}
	}

	connect(mHttp, &QNetworkAccessManager::finished,
		this, &DeezerClient::onNetworkAccessManagerFinished);
}

auto DeezerClient::login(const QString &arl, const QDateTime &expiration) const -> bool
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

	if (expiration.isValid())
	{
		cookie.setExpirationDate(expiration);
	}

	return cookies->insertCookie(cookie);
}

void DeezerClient::login(const QString &email, const QString &password)
{
	ApiResponse *response = gw().userData();
	connect(response, &ApiResponse::finished, this, [this, response, email, password]() -> void
	{
		if (!response->isValid())
		{
			qWarning() << response->errorString();
			response->deleteLater();
			emit loginFinished(LoginError::NoUserData);
			return;
		}

		const auto userData = response->value<UserData>();
		response->deleteLater();

		if (userData.checkFormLogin().isEmpty())
		{
			emit loginFinished(LoginError::NoCheckFormLogin);
			return;
		}

		const QUrl url(QStringLiteral("https://www.deezer.com/ajax/action.php"));

		const QString body = QStringLiteral("type=login&mail=%1&password=%2&checkFormLogin=%3")
			.arg(email, password, userData.checkFormLogin());

		QNetworkRequest request(url);
		request.setHeader(
			QNetworkRequest::ContentTypeHeader,
			QStringLiteral("application/x-www-form-urlencoded; charset=UTF-8")
		);

		QNetworkReply *reply = mHttp->post(request, body.toUtf8());
		connect(reply, &QNetworkReply::finished, reply, [this, reply]() -> void
		{
			if (reply->error() != QNetworkReply::NoError)
			{
				reply->deleteLater();
				emit loginFinished(LoginError::NetworkError);
				return;
			}

			if (QString::fromUtf8(reply->readAll()) != QStringLiteral("success"))
			{
				reply->deleteLater();
				emit loginFinished(LoginError::InvalidCredentials);
				return;
			}

			for (const QByteArray &data: reply->headers().values(QHttpHeaders::WellKnownHeader::SetCookie))
			{
				const QString value = QString::fromUtf8(data);

				const QString arl = cookieValue(value, QStringLiteral("arl"));
				if (arl.isEmpty())
				{
					continue;
				}

				QDateTime arlExpiration;
				if (const QString expires = cookieValue(value, QStringLiteral("expires")); !expires.isEmpty())
				{
					arlExpiration = QDateTime::fromString(expires,
						QStringLiteral("ddd, dd MMM yyyy HH:mm:ss tttt"));
				}

				if (!login(arl, arlExpiration))
				{
					reply->deleteLater();
					emit loginFinished(LoginError::InvalidCookie);
					return;
				}

				reply->deleteLater();
				emit loginFinished(LoginError::NoError);
				return;
			}

			reply->deleteLater();
			emit loginFinished(LoginError::NoArl);
		});
	});
}

auto DeezerClient::arl() const -> QString
{
	return QString::fromUtf8(arlCookie().value());
}

auto DeezerClient::arlExpiration() const -> QDateTime
{
	return arlCookie().expirationDate();
}

auto DeezerClient::get(const QUrl &url) -> ApiResponse *
{
	const QNetworkRequest request(url);
	QNetworkReply *reply = mHttp->get(request);
	return new ApiResponse(reply, this);
}

auto DeezerClient::gw() -> DeezerGw &
{
	return mGw;
}

auto DeezerClient::api() -> DeezerApi &
{
	return mApi;
}

auto DeezerClient::media() -> DeezerMedia &
{
	return mMedia;
}

auto DeezerClient::request(const QUrl &url) -> QNetworkRequest
{
	QNetworkRequest request(url);
	request.setHeaders(headers());
	return request;
}

auto DeezerClient::headers() -> QHttpHeaders
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

auto DeezerClient::cookieValue(const QString &value, const QString &key) -> QString
{
	const qsizetype begin = value.indexOf(QStringLiteral("%1=").arg(key));
	if (begin < 0)
	{
		return {};
	}

	const qsizetype end = value.indexOf(QStringLiteral(";"), begin);
	if (end < 0)
	{
		return {};
	}

	const qsizetype prefixLength = key.length() + 1;
	return value.mid(begin + prefixLength, end - begin - prefixLength);
}

auto DeezerClient::arlCookie() const -> QNetworkCookie
{
	const QUrl url(QStringLiteral("https://www.deezer.com"));
	for (const QNetworkCookie &cookie: mHttp->cookieJar()->cookiesForUrl(url))
	{
		if (cookie.name() != QStringLiteral("arl").toUtf8())
		{
			continue;
		}

		return cookie;
	}

	return QNetworkCookie();
}

void DeezerClient::onNetworkAccessManagerFinished(const QNetworkReply *reply) const
{
	const QVariant cookies = reply->header(QNetworkRequest::SetCookieHeader);
	for (const QNetworkCookie &cookie: cookies.value<QList<QNetworkCookie>>())
	{
		if (cookie.name() == QStringLiteral("sid").toUtf8()
			|| cookie.name() == QStringLiteral("dzr_uniq_id").toUtf8())
		{
			mHttp->cookieJar()->insertCookie(cookie);
		}
	}
}

void DeezerClient::createInstance(QObject *parent)
{
	sInstance = new DeezerClient(parent);
}

auto DeezerClient::instance() -> DeezerClient *
{
	return sInstance;
}
