#include "deezer/api/gwrequest.hpp"

GwRequest::GwRequest()
	: mBody(QStringLiteral("{}").toUtf8())
{
	mQuery.setQueryItems({
		{QStringLiteral("api_version"), QStringLiteral("1.0")},
		{QStringLiteral("api_token"), QString()},
		{QStringLiteral("input"), QStringLiteral("3")},
	});
}

auto GwRequest::query() const -> const QUrlQuery &
{
	return mQuery;
}

auto GwRequest::body() const -> const QByteArray &
{
	return mBody;
}

void GwRequest::setApiToken(const QString &token)
{
	mQuery.removeQueryItem(QStringLiteral("api_token"));
	mQuery.addQueryItem(QStringLiteral("api_token"), token);
}

void GwRequest::setMethod(const QString &method)
{
	mQuery.addQueryItem(QStringLiteral("method"), method);
}

void GwRequest::setGatewayInput(const QString &input)
{
	mQuery.addQueryItem(QStringLiteral("gateway_input"), input);
}

void GwRequest::setGatewayInput(const QJsonDocument &input)
{
	setGatewayInput(QString::fromUtf8(input.toJson(QJsonDocument::Compact)));
}

void GwRequest::setGatewayInput(const QJsonObject &input)
{
	setGatewayInput(QJsonDocument(input));
}

void GwRequest::setBody(const QJsonDocument &json)
{
	mBody = json.toJson(QJsonDocument::Compact);
}

void GwRequest::setBody(const QJsonObject &json)
{
	setBody(QJsonDocument(json));
}
