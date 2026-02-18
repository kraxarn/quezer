#pragma once

#include <QHttpHeaders>
#include <QNetworkAccessManager>

class DeezerApi : public QObject
{
	Q_OBJECT

public:
	explicit DeezerApi(QObject *parent);

	auto login(const QString &arl) -> bool;

private:
	QNetworkAccessManager mHttp;

	[[nodiscard]]
	auto gwApiCall(const QString &method) -> QNetworkReply *;

	[[nodiscard]]
	auto request(const QUrl &url) const -> QNetworkRequest;

	[[nodiscard]]
	auto headers() const -> QHttpHeaders;
};
