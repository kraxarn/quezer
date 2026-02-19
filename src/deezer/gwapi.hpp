#pragma once

#include "deezer/apiresponse.hpp"
#include "deezer/userdata.hpp"

#include <QNetworkAccessManager>
#include <QObject>

class GwApi : public QObject
{
	Q_OBJECT

public:
	GwApi(QNetworkAccessManager *http, QObject *parent);

	[[nodiscard]]
	auto userData() -> ApiResponse*;

private:
	QNetworkAccessManager *mHttp;

	[[nodiscard]]
	auto call(const QString &method) const -> QNetworkReply *;
};
