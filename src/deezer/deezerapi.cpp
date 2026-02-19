#include "deezer/deezerapi.hpp"

DeezerApi::DeezerApi(QNetworkAccessManager *http, QObject *parent)
	: QObject(parent),
	mHttp(http)
{
}
