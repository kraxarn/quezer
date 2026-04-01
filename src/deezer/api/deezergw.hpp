#pragma once

#include "deezer/apiresponse.hpp"
#include "deezer/api/gwrequest.hpp"
#include "deezer/objects/userdata.hpp"

#include <QNetworkAccessManager>
#include <QObject>

class DeezerGw : public QObject
{
	Q_OBJECT

public:
	DeezerGw(QNetworkAccessManager *http, QObject *parent);

	[[nodiscard]]
	auto userData() -> ApiResponse *;

	[[nodiscard]]
	auto songData(const UserData &userData, qint64 sngId) -> ApiResponse *;

	[[nodiscard]]
	auto homePage(const UserData &userData) -> ApiResponse *;

private:
	QNetworkAccessManager *mHttp;

	[[nodiscard]]
	auto call(const GwRequest &request) const -> QNetworkReply *;
};
