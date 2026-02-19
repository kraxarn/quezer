#pragma once

#include "deezer/deezerapi.hpp"
#include "deezer/deezergw.hpp"

#include <QHttpHeaders>
#include <QNetworkAccessManager>

class DeezerClient : public QObject
{
	Q_OBJECT

public:
	explicit DeezerClient(QObject *parent);

	auto login(const QString &arl) -> bool;

private:
	QNetworkAccessManager *mHttp;
	DeezerGw *mGw;
	DeezerApi *mApi;

	[[nodiscard]]
	auto request(const QUrl &url) const -> QNetworkRequest;

	[[nodiscard]]
	auto headers() const -> QHttpHeaders;
};
