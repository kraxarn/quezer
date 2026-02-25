#pragma once

#include "deezer/api/deezerapi.hpp"
#include "deezer/api/deezergw.hpp"
#include "deezer/api/deezermedia.hpp"
#include "deezer/enums/loginerror.hpp"

#include <QHttpHeaders>
#include <QNetworkAccessManager>
#include <qqmlintegration.h>

class DeezerClient : public QObject
{
	Q_OBJECT
	QML_ELEMENT

public:
	explicit DeezerClient(QObject *parent);

	void login(const QString &email, const QString &password);

	[[nodiscard]]
	auto get(const QUrl &url) -> ApiResponse *;

	[[nodiscard]]
	auto gw() -> DeezerGw &;

	[[nodiscard]]
	auto api() -> DeezerApi &;

	[[nodiscard]]
	auto media() -> DeezerMedia &;

signals:
	void loginFinished(LoginError error);

private:
	QNetworkAccessManager *mHttp;
	DeezerGw mGw;
	DeezerApi mApi;
	DeezerMedia mMedia;

	[[nodiscard]]
	auto login(const QString &arl) const -> bool;

	[[nodiscard]]
	static auto request(const QUrl &url) -> QNetworkRequest;

	[[nodiscard]]
	static auto headers() -> QHttpHeaders;
};
