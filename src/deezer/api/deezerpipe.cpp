#include "deezer/api/deezerpipe.hpp"

#include <QJsonObject>

DeezerPipe::DeezerPipe(QNetworkAccessManager *http, QObject *parent)
	: QObject(parent),
	mHttp(http)
{
}

auto DeezerPipe::albumFull(const qint64 albumId) -> ApiResponse *
{
	const QString operation = QStringLiteral("AlbumFull");
	const QString query = QStringLiteral(
		"query AlbumFull($albumId: String!) {\n"
		"  album(albumId: $albumId) {\n"
		"    __typename\n"
		"  }\n"
		"}"
	);
	const QVariantMap variables({
		{QStringLiteral("albumId"), QString::number(albumId)},
	});

	QNetworkReply *reply = call(operation, query, variables);
	return new ApiResponse(reply, this);
}

auto DeezerPipe::call(const QString &operation, const QString &query,
	const QVariantMap &variables) const -> QNetworkReply *
{
	const QUrl url(QStringLiteral("https://pipe.deezer.com/api"));

	QJsonObject body;
	body[QStringLiteral("operationName")] = operation;
	body[QStringLiteral("query")] = query;
	body[QStringLiteral("variables")] = QJsonObject::fromVariantMap(variables);

	qDebug().noquote() << QJsonDocument(body).toJson(QJsonDocument::Compact);

	const QNetworkRequest request(url);
	return mHttp->post(request, QJsonDocument(body).toJson(QJsonDocument::Compact));
}
