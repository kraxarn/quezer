#include "models/sidebarmodel.hpp"

SidebarModel::SidebarModel(QObject *parent)
	: QAbstractListModel(parent),
	mItems({
		SidebarItem(QStringLiteral("go-home"), QStringLiteral("Home"), false),
		SidebarItem(QStringLiteral("audio-card"), QStringLiteral("Explore"), false),       // TODO: Temporary icon
		SidebarItem(QStringLiteral("mail-mark-important"), QStringLiteral("Home"), false), // TODO: Temporary icon
		SidebarItem(QStringLiteral("document-new"), QStringLiteral("Playlists"), true)
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
