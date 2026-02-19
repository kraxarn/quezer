#include "deezer/apiresponse.hpp"

#include <QThreadPool>

ApiResponse::ApiResponse(QNetworkReply *reply, QObject *parent)
	: QObject(parent),
	mReply(reply)
{
	connect(reply, &QNetworkReply::finished,
		this, &ApiResponse::onReplyFinished);
}

auto ApiResponse::isValid() const -> bool
{
	return mReply == nullptr
		&& mParseError.error == QJsonParseError::NoError
		&& !mValue.isNull();
}

auto ApiResponse::errorString() const -> QString
{
	if (mReply->error() != QNetworkReply::NoError)
	{
		return mReply->errorString();
	}

	if (mParseError.error != QJsonParseError::NoError)
	{
		return mParseError.errorString();
	}

	return QString();
}

void ApiResponse::onReplyFinished()
{
	if (mReply->error() != QNetworkReply::NoError)
	{
		emit finished();
		return;
	}

	QThreadPool::globalInstance()->start([this]() -> void
	{
		const QByteArray data = mReply->readAll();
		mReply->deleteLater();
		mReply = nullptr;

		mValue = QJsonDocument::fromJson(data, &mParseError);
		emit finished();
	});
}
