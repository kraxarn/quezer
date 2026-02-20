#include "deezer/api/deezermedia.hpp"

#include <QJsonArray>

DeezerMedia::DeezerMedia(QNetworkAccessManager *http, QObject *parent)
	: QObject(parent),
	mHttp(http)
{
}

auto DeezerMedia::url(const UserData &userData, const SongData &songData,
	const MediaFormat mediaFormat) -> ApiResponse *
{
	const QUrl url(QStringLiteral("https://media.deezer.com/v1/get_url"));

	QJsonObject body;
	body[QStringLiteral("license_token")] = userData.licenseToken();
	body[QStringLiteral("track_tokens")] = QJsonArray({songData.trackToken()});

	body[QStringLiteral("media")] = QJsonArray({
		QJsonObject({
			{QStringLiteral("type"), QStringLiteral("FULL")},
			{
				QStringLiteral("formats"), QJsonArray({
					QJsonObject({
						{QStringLiteral("cipher"), QStringLiteral("BF_CBC_STRIPE")},
						{QStringLiteral("format"), mediaFormatString(mediaFormat)},
					}),
				})
			}
		}),
	});

	const QNetworkRequest request(url);
	QNetworkReply *reply = mHttp->post(request, QJsonDocument(body).toJson(QJsonDocument::Compact));
	return new ApiResponse(reply, this);
}

auto DeezerMedia::mediaFormatString(const MediaFormat mediaFormat) -> QString
{
	switch (mediaFormat)
	{
		case MediaFormat::LowQuality:
			return QStringLiteral("MP3_128");

		case MediaFormat::HighQuality:
			return QStringLiteral("MP3_320");

		case MediaFormat::Lossless:
			return QStringLiteral("FLAC");

		default:
			return QString();
	}
}
