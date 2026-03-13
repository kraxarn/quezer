#include "models/homepagemodel.hpp"
#include "deezer/deezerclient.hpp"
#include "models/homepageitemmodel.hpp"
#include "qml/pagefilteroption.hpp"

HomePageModel::HomePageModel(QObject *parent)
	: QAbstractListModel(parent)
{
	const ApiResponse *response = DeezerClient::instance()->gw().userData();
	connect(response, &ApiResponse::finished,
		this, &HomePageModel::onUserData);
}

auto HomePageModel::roleNames() const -> QHash<int, QByteArray>
{
	return {
		{
			{static_cast<int>(ItemRole::Title), "title"},
			{static_cast<int>(ItemRole::Subtitle), "subtitle"},
			{static_cast<int>(ItemRole::FilterOption), "filterOption"},
			{static_cast<int>(ItemRole::FilterOptions), "filterOptions"},
			{static_cast<int>(ItemRole::Items), "items"},
		}
	};
}

auto HomePageModel::rowCount([[maybe_unused]] const QModelIndex &parent) const -> int
{
	return static_cast<int>(mPage.sections().length());
}

auto HomePageModel::data(const QModelIndex &index, const int role) const -> QVariant
{
	const Page::Section &section = mPage.sections().at(index.row());
	const auto itemRole = static_cast<ItemRole>(role);

	if (itemRole == ItemRole::Title)
	{
		return section.title();
	}

	if (itemRole == ItemRole::Subtitle)
	{
		return section.subtitle();
	}

	if (itemRole == ItemRole::FilterOption)
	{
		for (const Page::Section::Filter::Option &option: section.filter().options())
		{
			if (option.id() == section.filter().defaultOptionId())
			{
				return QVariant::fromValue(new PageFilterOption(option, parent()));
			}
		}

		return {};
	}

	if (itemRole == ItemRole::FilterOptions)
	{
		QList<PageFilterOption *> labels;
		labels.reserve(section.filter().options().length());

		for (const Page::Section::Filter::Option &option: section.filter().options())
		{
			labels.append(new PageFilterOption(option, parent()));
		}

		return QVariant::fromValue(labels);
	}

	if (itemRole == ItemRole::Items)
	{
		return QVariant::fromValue(section.items());
	}

	return {};
}

void HomePageModel::onUserData() const
{
	UserData userData;
	{
		auto *response = qobject_cast<ApiResponse *>(sender());
		if (!response->isValid())
		{
			qWarning() << "Failed to load user data:" << response->errorString();
			response->deleteLater();
			return;
		}

		userData = response->value<UserData>();
		response->deleteLater();
	}
	{
		const ApiResponse *response = DeezerClient::instance()->gw().homePage(userData);
		connect(response, &ApiResponse::finished,
			this, &HomePageModel::onHomePage);
	}
}

void HomePageModel::onHomePage()
{
	auto *response = qobject_cast<ApiResponse *>(sender());
	if (!response->isValid())
	{
		qWarning() << "Failed to load home page:" << response->errorString();
		response->deleteLater();
		return;
	}

	const auto page = response->value<Page>();

	beginInsertRows({}, 0, static_cast<int>(page.sections().length()) - 1);
	{
		mPage = page;
	}
	endInsertRows();
}
