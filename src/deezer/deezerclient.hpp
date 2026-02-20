#pragma once

#include "deezer/api/deezerapi.hpp"
#include "deezer/api/deezergw.hpp"
#include "deezer/api/deezermedia.hpp"

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

	[[nodiscard]]
	auto media() -> DeezerMedia &;

private:
	QNetworkAccessManager *mHttp;
	DeezerGw mGw;
	DeezerApi mApi;
	DeezerMedia mMedia;

	[[nodiscard]]
	auto request(const QUrl &url) const -> QNetworkRequest;

	[[nodiscard]]
	auto headers() const -> QHttpHeaders;
};
