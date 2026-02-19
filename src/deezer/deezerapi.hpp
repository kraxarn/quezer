#pragma once

#include "deezer/gwapi.hpp"

#include <QHttpHeaders>
#include <QNetworkAccessManager>

class DeezerApi : public QObject
{
	Q_OBJECT

public:
	explicit DeezerApi(QObject *parent);

	auto login(const QString &arl) -> bool;

private:
	QNetworkAccessManager *mHttp;
	GwApi *mGwApi;

	[[nodiscard]]
	auto request(const QUrl &url) const -> QNetworkRequest;

	[[nodiscard]]
	auto headers() const -> QHttpHeaders;
};
