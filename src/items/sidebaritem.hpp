#pragma once

#include <QString>

class SidebarItem final
{
public:
	SidebarItem(QString icon, QString text, bool action);

	[[nodiscard]]
	auto icon() const -> const QString &;

	[[nodiscard]]
	auto text() const -> const QString &;

	[[nodiscard]]
	auto action() const -> bool;

private:
	QString mIcon;
	QString mText;
	bool mAction;
};
