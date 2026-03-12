#pragma once

#include <qqmlintegration.h>
#include <QSortFilterProxyModel>

class HomePageItemProxy : public QSortFilterProxyModel
{
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(QString filterOptionId READ filterOptionId WRITE setFilterOptionId NOTIFY filterOptionIdChanged)

public:
	explicit HomePageItemProxy(QObject *parent = nullptr);

	[[nodiscard]]
	auto filterOptionId() const -> const QString &;

	void setFilterOptionId(const QString &optionId);

signals:
	void filterOptionIdChanged();

protected:
	[[nodiscard]]
	auto filterAcceptsRow(int sourceRow,
		const QModelIndex &sourceParent) const -> bool override;

private:
	QString mFilterOptionId;
};
