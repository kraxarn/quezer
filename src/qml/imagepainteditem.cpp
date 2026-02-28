#include "qml/imagepainteditem.hpp"

#include <QPainter>

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

	painter->drawImage(0, 0, mImage);
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
