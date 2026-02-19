#include "deezer/api/deezergw.hpp"

#include <QUrlQuery>

DeezerGw::DeezerGw(QNetworkAccessManager *http, QObject *parent)
	: QObject(parent),
	mHttp(http)
{
}

auto DeezerGw::userData() -> ApiResponse *
{
	QNetworkReply *reply = call(QStringLiteral("deezer.getUserData"));
	return new ApiResponse(reply, this);
}

auto DeezerGw::call(const QString &method) const -> QNetworkReply *
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

	QNetworkRequest request(url);
	request.setHeader(
		QNetworkRequest::UserAgentHeader,
		QStringLiteral(
			"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
		)
	);

	return mHttp->post(request, QStringLiteral("{}").toUtf8());
}
