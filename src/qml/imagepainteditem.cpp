#include "qml/imagepainteditem.hpp"

#include <QPainter>
#include <QPainterPath>

ImagePaintedItem::ImagePaintedItem(QQuickItem *parent)
	: QQuickPaintedItem(parent)
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
