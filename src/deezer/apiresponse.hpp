#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QObject>

class ApiResponse : public QObject
{
	Q_OBJECT

public:
	ApiResponse(QNetworkReply *reply, QObject *parent);

	[[nodiscard]]
	auto isValid() const -> bool;

	template<typename T>
	[[nodiscard]]
	auto value() const -> T
	{
		return T::fromJson(object());
	}

	[[nodiscard]]
	auto object() const -> QJsonObject;

	[[nodiscard]]
	auto errorString() const -> QString;

signals:
	void finished();

private:
	QNetworkReply *mReply;
	QJsonParseError mParseError;
	QJsonDocument mValue;

	void onReplyFinished();
};
