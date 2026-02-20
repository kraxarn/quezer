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
	auto userData() const -> ApiResponse *;

	[[nodiscard]]
	auto search(SearchMediaType mediaType, const QString &query,
		SearchMode mode = SearchMode::Fuzzy,
		SearchOrder order = SearchOrder::Ranking) const -> ApiResponse *;

	[[nodiscard]]
	auto album(qint64 albumId) const -> ApiResponse *;

	[[nodiscard]]
	auto gw() const -> DeezerGw &;

private:
	QNetworkAccessManager *mHttp;
	DeezerGw *mGw;
	DeezerApi *mApi;

	[[nodiscard]]
	auto request(const QUrl &url) const -> QNetworkRequest;

	[[nodiscard]]
	auto headers() const -> QHttpHeaders;
};
