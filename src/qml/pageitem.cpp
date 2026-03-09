#include "qml/pageitem.hpp"


PageItem::PageItem(QObject *parent)
	: QObject(parent)
{
}

PageItem::PageItem(const Page::Section::Item &item, QObject *parent)
	: QObject(parent),
	mItem(item)
{
}

auto PageItem::title() const -> const QString &
{
	return mItem.title();
}

auto PageItem::subtitle() const -> const QString &
{
	return mItem.subtitle();
}
