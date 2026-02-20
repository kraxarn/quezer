#include "deezer/api/deezergw.hpp"

#include <QUrlQuery>

DeezerGw::DeezerGw(QNetworkAccessManager *http, QObject *parent)
	: QObject(parent),
	mHttp(http)
{
}

auto DeezerGw::checkForm() const -> const QString &
{
	return mCheckForm;
}

void DeezerGw::setCheckForm(const QString &checkForm)
{
	mCheckForm = checkForm;
}

auto DeezerGw::userData() -> ApiResponse *
{
	QNetworkReply *reply = call(QStringLiteral("deezer.getUserData"));
	return new ApiResponse(reply, this);
}

auto DeezerGw::songData(const qint64 sngId) -> ApiResponse *
{
	QJsonObject body;
	body[QStringLiteral("SNG_ID")] = sngId;

	QNetworkReply *reply = call(QStringLiteral("song.getData"), QJsonDocument(body));
	return new ApiResponse(reply, this);
}

auto DeezerGw::call(const QString &method) const -> QNetworkReply *
{
	return call(method, QStringLiteral("{}").toUtf8());
}

auto DeezerGw::call(const QString &method, const QJsonDocument &body) const -> QNetworkReply *
{
	return call(method, body.toJson(QJsonDocument::Compact));
}

auto DeezerGw::call(const QString &method, const QByteArray &body) const -> QNetworkReply *
{
	const QString token = mCheckForm.isEmpty()
		? QStringLiteral("null")
		: mCheckForm;

	QUrl url(QStringLiteral("https://www.deezer.com/ajax/gw-light.php"));
	url.setQuery({
		{
			{QStringLiteral("api_version"), QStringLiteral("1.0")},
			{QStringLiteral("api_token"), token},
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

	return mHttp->post(request, body);
}
