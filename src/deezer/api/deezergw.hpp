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
	auto userData() -> ApiResponse*;

private:
	QNetworkAccessManager *mHttp;

	[[nodiscard]]
	auto call(const QString &method) const -> QNetworkReply *;
};
