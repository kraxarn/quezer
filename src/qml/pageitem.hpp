#pragma once

#include "deezer/objects/page.hpp"

#include <QObject>
#include <qqmlintegration.h>

class PageItem : public QObject
{
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(QString title READ title NOTIFY titleChanged)
	Q_PROPERTY(QString subtitle READ subtitle NOTIFY subtitleChanged)

public:
	explicit PageItem(QObject *parent = nullptr);

	explicit PageItem(const Page::Section::Item &item, QObject *parent = nullptr);

	[[nodiscard]]
	auto title() const -> const QString &;

	[[nodiscard]]
	auto subtitle() const -> const QString &;

signals:
	void titleChanged();

	void subtitleChanged();

private:
	Page::Section::Item mItem;
};

Q_DECLARE_METATYPE(PageItem);
