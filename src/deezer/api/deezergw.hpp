#pragma once

#include "deezer/apiresponse.hpp"
#include "deezer/objects/userdata.hpp"

#include <QNetworkAccessManager>
#include <QObject>

class DeezerGw : public QObject
{
	Q_OBJECT

public:
	DeezerGw(QNetworkAccessManager *http, QObject *parent);

	[[nodiscard]]
	auto checkForm() const -> const QString &;

	void setCheckForm(const QString &checkForm);

	[[nodiscard]]
	auto userData() -> ApiResponse *;

	[[nodiscard]]
	auto songData(const UserData &userData, qint64 sngId) -> ApiResponse *;

private:
	QNetworkAccessManager *mHttp;

	[[nodiscard]]
	auto call(const QString &method) const -> QNetworkReply *;

	[[nodiscard]]
	auto call(const QString &method, const QJsonDocument &body,
		const QString &token) const -> QNetworkReply *;

	[[nodiscard]]
	auto call(const QString &method, const QByteArray &body,
		const QString &token) const -> QNetworkReply *;
};
