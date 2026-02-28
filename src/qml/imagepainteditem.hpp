#pragma once

#include <QImage>
#include <qqmlintegration.h>
#include <QQuickPaintedItem>

class ImagePaintedItem : public QQuickPaintedItem
{
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)

public:
	explicit ImagePaintedItem(QQuickItem *parent = nullptr);

	void paint(QPainter *painter) override;

	[[nodiscard]]
	auto image() const -> const QImage &;

	void setImage(const QImage &image);

signals:
	void imageChanged();

private:
	QImage mImage;
};
