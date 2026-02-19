#pragma once

#include "deezer/gwapi.hpp"
#include "enums/searchmediatype.hpp"

#include <QHttpHeaders>
#include <QNetworkAccessManager>

class DeezerClient : public QObject
{
	Q_OBJECT

public:
	explicit DeezerClient(QObject *parent);

	auto login(const QString &arl) -> bool;

	void search(SearchMediaType mediaType, const QString &query);

private:
	QNetworkAccessManager *mHttp;
	GwApi *mGwApi;

	[[nodiscard]]
	auto request(const QUrl &url) const -> QNetworkRequest;

	[[nodiscard]]
	auto headers() const -> QHttpHeaders;
};
