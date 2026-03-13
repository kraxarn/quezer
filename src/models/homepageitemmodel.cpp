#include "models/homepageitemmodel.hpp"

HomePageItemModel::HomePageItemModel(QObject *parent)
	: QAbstractListModel(parent)
{
}

auto HomePageItemModel::roleNames() const -> QHash<int, QByteArray>
{
	return {
		{
			{static_cast<int>(ItemRole::Title), "title"},
			{static_cast<int>(ItemRole::Subtitle), "subtitle"},
			{static_cast<int>(ItemRole::PictureUrl), "pictureUrl"},
			{static_cast<int>(ItemRole::PictureSize), "pictureSize"},
			{static_cast<int>(ItemRole::PictureRadius), "pictureRadius"},
		}
	};
}

auto HomePageItemModel::rowCount([[maybe_unused]] const QModelIndex &parent) const -> int
{
	return static_cast<int>(mItems.length());
}

auto HomePageItemModel::data(const QModelIndex &index, int role) const -> QVariant
{
	const Page::Section::Item &item = mItems.at(index.row());
	const auto itemRole = static_cast<ItemRole>(role);

	if (itemRole == ItemRole::Title)
	{
		return item.title();
	}

	if (itemRole == ItemRole::Subtitle)
	{
		return item.subtitle();
	}

	if (itemRole == ItemRole::PictureUrl)
	{
		if (!item.pictures().isEmpty())
		{
			return item.pictures().first().url();
		}

		if (!item.imageLinkedItem().md5().isEmpty())
		{
			return item.imageLinkedItem().url();
		}

		return QUrl();
	}

	if (itemRole == ItemRole::PictureSize)
	{
		return pictureSize(item);
	}

	if (itemRole == ItemRole::PictureRadius)
	{
		static constexpr auto radiusDefault = 6;

		if (item.type() == QStringLiteral("flow")
			|| item.type() == QStringLiteral("artist"))
		{
			return pictureSize(item) / 2;
		}

		return radiusDefault;
	}

	return {};
}

auto HomePageItemModel::items() const -> const QList<Page::Section::Item> &
{
	return mItems;
}

void HomePageItemModel::setItems(const QList<Page::Section::Item> &items)
{
	if (!mItems.isEmpty())
	{
		beginRemoveRows({}, 0, static_cast<int>(mItems.length()) - 1);
		{
			mItems.clear();
		}
		endRemoveRows();
	}

	beginInsertRows({}, 0, static_cast<int>(items.length()) - 1);
	{
		mItems.append(items);
	}
	endInsertRows();
}

auto HomePageItemModel::pictureSize(const Page::Section::Item &item) -> int
{
	static constexpr auto sizeSmall = 80;
	static constexpr auto sizeLarge = 140;

	if (item.type() == QStringLiteral("flow"))
	{
		return sizeSmall;
	}

	return sizeLarge;
}
