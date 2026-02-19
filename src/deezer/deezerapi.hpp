#pragma once

#include <QNetworkAccessManager>
#include <QObject>

class DeezerApi : public QObject
{
	Q_OBJECT

public:
	DeezerApi(QNetworkAccessManager *http, QObject *parent);

private:
	QNetworkAccessManager *mHttp;
};
