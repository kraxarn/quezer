#include "models/homepageitemmodel.hpp"

HomePageItemModel::HomePageItemModel(QObject *parent)
	: QAbstractListModel(parent)
{
}

QHash<int, QByteArray> HomePageItemModel::roleNames() const
{
	return {
		{
			{static_cast<int>(ItemRole::Title), "title"},
			{static_cast<int>(ItemRole::PictureUrl), "pictureUrl"},
		}
	};
}

auto HomePageItemModel::rowCount(const QModelIndex &parent) const -> int
{
	return static_cast<int>(mItems.length());
}

auto HomePageItemModel::data(const QModelIndex &index, int role) const -> QVariant
{
	const Page::Section::Item &item = mItems.at(index.row());

	switch (static_cast<ItemRole>(role))
	{
		case ItemRole::Title:
			return item.title();

		case ItemRole::PictureUrl:
			return item.pictures().isEmpty()
				? QUrl()
				: item.pictures().first().url();

		default:
			return {};
	}
}

auto HomePageItemModel::items() const -> const QList<Page::Section::Item> &
{
	return mItems;
}

void HomePageItemModel::setItems(const QList<Page::Section::Item> &items)
{
	if (!mItems.isEmpty())
	{
		beginRemoveRows({}, 0, mItems.length() - 1);
		{
			mItems.clear();
		}
		endRemoveRows();
	}

	beginInsertRows({}, 0, items.length() - 1);
	{
		mItems.append(items);
	}
	endInsertRows();
}
