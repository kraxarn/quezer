#pragma once

#include "deezer/objects/page.hpp"
#include "deezer/objects/userdata.hpp"

#include <QAbstractListModel>

class HomePageModel : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit HomePageModel(QObject *parent = nullptr);

	[[nodiscard]]
	auto roleNames() const -> QHash<int, QByteArray> override;

	[[nodiscard]]
	auto rowCount(const QModelIndex &parent) const -> int override;

	[[nodiscard]]
	auto data(const QModelIndex &index, int role) const -> QVariant override;

private:
	enum class ItemRole: quint16
	{
		Title = Qt::UserRole + 1,
		Subtitle,
	};

	Page mPage;

	void onUserData() const;

	void onHomePage();
};
