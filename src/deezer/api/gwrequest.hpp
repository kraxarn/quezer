#pragma once

#include <QJsonObject>
#include <QUrlQuery>

class GwRequest final
{
public:
	GwRequest();

	[[nodiscard]]
	auto query() const -> const QUrlQuery &;

	[[nodiscard]]
	auto body() const -> const QByteArray &;

	void setApiToken(const QString &token);

	void setMethod(const QString &method);

	void setGatewayInput(const QString &input);

	void setGatewayInput(const QJsonDocument &input);

	void setGatewayInput(const QJsonObject &input);

	void setBody(const QJsonDocument &json);

	void setBody(const QJsonObject &json);

private:
	QUrlQuery mQuery;
	QByteArray mBody;
};
