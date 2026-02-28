#include "models/sidebarmodel.hpp"

SidebarModel::SidebarModel(QObject *parent)
	: QAbstractListModel(parent),
	mItems({
		SidebarItem(QStringLiteral("home-outline"), QStringLiteral("Home"), false),
		SidebarItem(QStringLiteral("compass-outline"), QStringLiteral("Explore"), false),       // TODO: Temporary icon
		SidebarItem(QStringLiteral("heart-outline"), QStringLiteral("Favourites"), false), // TODO: Temporary icon
		SidebarItem(QStringLiteral("plus"), QStringLiteral("Playlists"), true)
	})
{
}

auto SidebarModel::roleNames() const -> QHash<int, QByteArray>
{
	return {
		{
			{static_cast<int>(ItemRole::Icon), "icon"},
			{static_cast<int>(ItemRole::Text), "text"},
			{static_cast<int>(ItemRole::Action), "action"},
		}
	};
}

auto SidebarModel::rowCount([[maybe_unused]] const QModelIndex &parent) const -> int
{
	return static_cast<int>(mItems.size());
}

auto SidebarModel::data(const QModelIndex &index, int role) const -> QVariant
{
	const SidebarItem &item = mItems.at(index.row());

	switch (static_cast<ItemRole>(role))
	{
		case ItemRole::Icon:
			return item.icon();

		case ItemRole::Text:
			return item.text();

		case ItemRole::Action:
			return item.action();

		default:
			return {};
	}
}
