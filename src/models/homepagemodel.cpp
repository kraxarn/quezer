#include "models/homepagemodel.hpp"
#include "deezer/deezerclient.hpp"

HomePageModel::HomePageModel(QObject *parent)
	: QAbstractListModel(parent)
{
	const ApiResponse *response = DeezerClient::instance()->gw().userData();
	connect(response, &ApiResponse::finished,
		this, &HomePageModel::onUserData);
}

QHash<int, QByteArray> HomePageModel::roleNames() const
{
	return {
		{
			{static_cast<int>(ItemRole::Title), "title"},
			{static_cast<int>(ItemRole::Subtitle), "subtitle"},
		}
	};
}

auto HomePageModel::rowCount(const QModelIndex &parent) const -> int
{
	return static_cast<int>(mPage.sections().length());
}

auto HomePageModel::data(const QModelIndex &index, const int role) const -> QVariant
{
	const Page::Section &section = mPage.sections().at(index.row());

	switch (static_cast<ItemRole>(role))
	{
		case ItemRole::Title:
			return section.title();

		case ItemRole::Subtitle:
			return section.subtitle();

		default:
			return {};
	}
}

void HomePageModel::onUserData() const
{
	UserData userData;
	{
		const auto response = qobject_cast<ApiResponse *>(sender());
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
	const auto response = qobject_cast<ApiResponse *>(sender());
	if (!response->isValid())
	{
		qWarning() << "Failed to load home page:" << response->errorString();
		response->deleteLater();
		return;
	}

	const auto page = response->value<Page>();

	beginInsertRows({}, 0, page.sections().length() - 1);
	{
		mPage = page;
	}
	endInsertRows();
}
