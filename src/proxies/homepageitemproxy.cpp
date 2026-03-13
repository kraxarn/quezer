#include "proxies/homepageitemproxy.hpp"
#include "models/homepageitemmodel.hpp"

HomePageItemProxy::HomePageItemProxy(QObject *parent)
	: QSortFilterProxyModel(parent)
{
}

auto HomePageItemProxy::filterOptionId() const -> const QString &
{
	return mFilterOptionId;
}

void HomePageItemProxy::setFilterOptionId(const QString &optionId)
{
	beginFilterChange();
	{
		mFilterOptionId = optionId;
	}
	endFilterChange();

	emit filterOptionIdChanged();
}

auto HomePageItemProxy::filterAcceptsRow(const int sourceRow,
	[[maybe_unused]] const QModelIndex &sourceParent) const -> bool
{
	if (filterOptionId().isEmpty())
	{
		return true;
	}

	const auto *model = qobject_cast<HomePageItemModel *>(sourceModel());
	if (model == nullptr)
	{
		return true;
	}

	const Page::Section::Item &item = model->items().at(sourceRow);
	if (item.filterOptionIds().isEmpty())
	{
		return true;
	}

	return std::ranges::any_of(item.filterOptionIds(),
		[this](const QString &optionId) -> bool
		{
			return optionId == filterOptionId();
		});
}
