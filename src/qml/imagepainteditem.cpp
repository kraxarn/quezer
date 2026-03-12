#include "qml/imagepainteditem.hpp"

#include <QNetworkReply>
#include <QPainter>
#include <QPainterPath>

ImagePaintedItem::ImagePaintedItem(QQuickItem *parent)
	: QQuickPaintedItem(parent),
	mHttp(this)
{
}

void ImagePaintedItem::paint(QPainter *painter)
{
	if (mImage.isNull())
	{
		return;
	}

	const QSize size = painter->viewport().size();
	const QImage scaledImage = mImage.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

	const QQuickItem *parent = parentItem();
	const qreal radius = parent->property("radius").toReal();

	QPainterPath path({0.F, 0.F});
	path.addRoundedRect({{}, size}, radius, radius);

	painter->setClipPath(path);
	painter->drawImage(0, 0, scaledImage);
}

auto ImagePaintedItem::image() const -> const QImage &
{
	return mImage;
}

void ImagePaintedItem::setImage(const QImage &image)
{
	mImage = image;
	emit imageChanged();

	if (!mImage.isNull())
	{
		update();
	}
}

auto ImagePaintedItem::source() const -> const QUrl &
{
	return mSource;
}

void ImagePaintedItem::setSource(const QUrl &source)
{
	mSource = source;
	emit sourceChanged();

	if (!source.isValid())
	{
		return;
	}

	const QNetworkRequest request(mSource);
	const QNetworkReply *reply = mHttp.get(request);

	connect(reply, &QNetworkReply::finished,
		this, &ImagePaintedItem::onReplyFinished);
}

void ImagePaintedItem::onReplyFinished()
{
	const auto reply = qobject_cast<QNetworkReply *>(sender());
	if (reply->error() != QNetworkReply::NoError)
	{
		qWarning() << "Invalid source:" << reply->errorString();
		reply->deleteLater();
		return;
	}

	setImage(QImage::fromData(reply->readAll()));
	reply->deleteLater();
}
