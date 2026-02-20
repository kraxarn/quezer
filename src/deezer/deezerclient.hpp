#pragma once

#include "deezer/api/deezerapi.hpp"
#include "deezer/api/deezergw.hpp"

#include <QHttpHeaders>
#include <QNetworkAccessManager>

class DeezerClient : public QObject
{
	Q_OBJECT

public:
	explicit DeezerClient(QObject *parent);

	auto login(const QString &arl) const -> bool;

	[[nodiscard]]
	auto gw() -> DeezerGw &;

	[[nodiscard]]
	auto api() -> DeezerApi &;

private:
	QNetworkAccessManager *mHttp;
	DeezerGw mGw;
	DeezerApi mApi;

	[[nodiscard]]
	auto request(const QUrl &url) const -> QNetworkRequest;

	[[nodiscard]]
	auto headers() const -> QHttpHeaders;
};
