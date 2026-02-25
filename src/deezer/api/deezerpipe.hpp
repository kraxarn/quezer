#pragma once

#include "deezer/apiresponse.hpp"

#include <QNetworkAccessManager>
#include <QObject>

class DeezerPipe final : public QObject
{
	Q_OBJECT

public:
	DeezerPipe(QNetworkAccessManager *http, QObject *parent);

	[[nodiscard]]
	auto albumFull(qint64 albumId) -> ApiResponse *;

private:
	QNetworkAccessManager *mHttp;

	[[nodiscard]]
	auto call(const QString &operation, const QString &query,
		const QVariantMap &variables) const -> QNetworkReply *;
};
