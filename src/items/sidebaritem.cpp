#include "items/sidebaritem.hpp"

#include <utility>

SidebarItem::SidebarItem(QString icon, QString text, const bool action)
	: mIcon(std::move(icon)),
	mText(std::move(text)),
	mAction(action)
{
}

auto SidebarItem::icon() const -> const QString &
{
	return mIcon;
}

auto SidebarItem::text() const -> const QString &
{
	return mText;
}

auto SidebarItem::action() const -> bool
{
	return mAction;
}
