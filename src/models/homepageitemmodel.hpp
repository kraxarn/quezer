#pragma once

#include "deezer/objects/page.hpp"

#include <QAbstractListModel>
#include <qqmlintegration.h>

class HomePageItemModel : public QAbstractListModel
{
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(QList<Page::Section::Item> items READ items WRITE setItems NOTIFY itemsChanged)

public:
	explicit HomePageItemModel(QObject *parent = nullptr);

	[[nodiscard]]
	auto roleNames() const -> QHash<int, QByteArray> override;

	[[nodiscard]]
	auto rowCount(const QModelIndex &parent) const -> int override;

	[[nodiscard]]
	auto data(const QModelIndex &index, int role) const -> QVariant override;

	[[nodiscard]]
	auto items() const -> const QList<Page::Section::Item> &;

	void setItems(const QList<Page::Section::Item> &items);

signals:
	void itemsChanged();

private:
	enum class ItemRole: quint16
	{
		Title = Qt::UserRole + 1,
		PictureUrl,
	};

	QList<Page::Section::Item> mItems;
};
