#pragma once

#include "deezer/apiresponse.hpp"
#include "deezer/enums/searchmediatype.hpp"
#include "deezer/enums/searchmode.hpp"
#include "deezer/enums/searchorder.hpp"

#include <QNetworkAccessManager>
#include <QObject>

class DeezerApi : public QObject
{
	Q_OBJECT

public:
	DeezerApi(QNetworkAccessManager *http, QObject *parent);

	[[nodiscard]]
	auto search(SearchMediaType mediaType, const QString &query,
		SearchMode mode = SearchMode::Fuzzy,
		SearchOrder order = SearchOrder::Ranking) -> ApiResponse *;

	[[nodiscard]]
	auto album(qint64 albumId) -> ApiResponse *;

private:
	QNetworkAccessManager *mHttp;

	[[nodiscard]]
	auto call(const QString &path, const QUrlQuery &args) const -> QNetworkReply *;

	[[nodiscard]]
	static auto searchMediaTypeString(SearchMediaType mediaType) -> QString;

	[[nodiscard]]
	static auto searchModeString(SearchMode mode) -> QString;

	[[nodiscard]]
	static auto searchOrderString(SearchOrder order) -> QString;
};
