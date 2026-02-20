#pragma once

#include "deezer/apiresponse.hpp"
#include "deezer/enums/mediaformat.hpp"
#include "deezer/objects/songdata.hpp"
#include "deezer/objects/userdata.hpp"

#include <QNetworkAccessManager>
#include <QObject>

class DeezerMedia final : public QObject
{
	Q_OBJECT

public:
	DeezerMedia(QNetworkAccessManager *http, QObject *parent);

	[[nodiscard]]
	auto url(const UserData &userData, const SongData &songData,
		MediaFormat mediaFormat) -> ApiResponse *;

private:
	QNetworkAccessManager *mHttp;

	[[nodiscard]]
	static auto mediaFormatString(MediaFormat mediaFormat) -> QString;
};
