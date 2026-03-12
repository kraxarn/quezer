#pragma once

#include <QImage>
#include <QNetworkAccessManager>
#include <qqmlintegration.h>
#include <QQuickPaintedItem>

class ImagePaintedItem : public QQuickPaintedItem
{
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
	Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

public:
	explicit ImagePaintedItem(QQuickItem *parent = nullptr);

	void paint(QPainter *painter) override;

	[[nodiscard]]
	auto image() const -> const QImage &;

	void setImage(const QImage &image);

	[[nodiscard]]
	auto source() const -> const QUrl &;

	void setSource(const QUrl &source);

signals:
	void imageChanged();

	void sourceChanged();

private:
	QNetworkAccessManager mHttp;

	QImage mImage;
	QUrl mSource;

	void onReplyFinished();
};
