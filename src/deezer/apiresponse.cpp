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
		&& !mValue.isNull()
		&& mValue.object().value(QStringLiteral("error")).toObject().isEmpty();
}

auto ApiResponse::data() const -> const QByteArray &
{
	return mData;
}

auto ApiResponse::object() const -> QJsonObject
{
	return mValue.object();
}

auto ApiResponse::errorString() const -> QString
{
	if (mReply != nullptr && mReply->error() != QNetworkReply::NoError)
	{
		return mReply->errorString();
	}

	if (mParseError.error != QJsonParseError::NoError)
	{
		return mParseError.errorString();
	}

	if (!mValue.object().value(QStringLiteral("error")).toObject().isEmpty())
	{
		return mValue.object()
			.value(QStringLiteral("error")).toObject()
			.constBegin().value().toString();
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
		mData = mReply->readAll();
		mReply->deleteLater();
		mReply = nullptr;

		mValue = QJsonDocument::fromJson(mData, &mParseError);
		emit finished();
	});
}
