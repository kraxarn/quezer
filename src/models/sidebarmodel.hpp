#pragma once

#include "items/sidebaritem.hpp"

#include <QAbstractListModel>

#include <array>

class SidebarModel : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit SidebarModel(QObject *parent = nullptr);

	[[nodiscard]]
	auto roleNames() const -> QHash<int, QByteArray> override;

	[[nodiscard]]
	auto rowCount(const QModelIndex &parent) const -> int override;

	[[nodiscard]]
	auto data(const QModelIndex &index, int role) const -> QVariant override;

private:
	std::array<SidebarItem, 4> mItems;

	enum class ItemRole: quint16
	{
		Icon = Qt::UserRole + 1,
		Text,
		Action,
	};
};
