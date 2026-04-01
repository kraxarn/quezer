#include "deezer/api/deezergw.hpp"

#include <QJsonArray>
#include <QUrlQuery>

DeezerGw::DeezerGw(QNetworkAccessManager *http, QObject *parent)
	: QObject(parent),
	mHttp(http)
{
}

auto DeezerGw::userData() -> ApiResponse *
{
	GwRequest request;
	request.setMethod(QStringLiteral("deezer.getUserData"));

	QNetworkReply *reply = call(request);
	return new ApiResponse(reply, this);
}

auto DeezerGw::songData(const UserData &userData, const qint64 sngId) -> ApiResponse *
{
	QJsonObject body;
	body[QStringLiteral("SNG_ID")] = sngId;

	GwRequest request;
	request.setMethod(QStringLiteral("song.getData"));
	request.setBody(body);
	request.setApiToken(userData.checkForm());

	QNetworkReply *reply = call(request);
	return new ApiResponse(reply, this);
}

auto DeezerGw::homePage(const UserData &userData) -> ApiResponse *
{
	const QJsonArray supportedItems({
		QStringLiteral("artist"),
		QStringLiteral("playlist"),
		QStringLiteral("channel"), // genres etc.
		QStringLiteral("album"),
	});

	QJsonObject input;
	input[QStringLiteral("PAGE")] = QStringLiteral("home");
	input[QStringLiteral("VERSION")] = QStringLiteral("2.5");
	input[QStringLiteral("LANG")] = QStringLiteral("en");
	input[QStringLiteral("OPTIONS")] = QJsonArray();
	input[QStringLiteral("SUPPORT")] = QJsonObject({
		{
			QStringLiteral("filterable-grid"), QJsonArray({
				QStringLiteral("flow"),
			}),
		},
		{QStringLiteral("horizontal-grid"), supportedItems},
		{QStringLiteral("long-card-horizontal-grid"), supportedItems},
	});

	GwRequest request;
	request.setMethod(QStringLiteral("page.get"));
	request.setGatewayInput(input);
	request.setApiToken(userData.checkForm());

	QNetworkReply *reply = call(request);

	return new ApiResponse(reply, this);
}

auto DeezerGw::call(const GwRequest &request) const -> QNetworkReply *
{
	QUrl url(QStringLiteral("https://www.deezer.com/ajax/gw-light.php"));
	url.setQuery(request.query());

	return mHttp->post(QNetworkRequest(url), request.body());
}
