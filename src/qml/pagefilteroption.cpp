#include "qml/pagefilteroption.hpp"

PageFilterOption::PageFilterOption(QObject *parent)
	: QObject(parent)
{
}

PageFilterOption::PageFilterOption(const Page::Section::Filter::Option &option,
	QObject *parent)
	: QObject(parent),
	mOption(option)
{
}

auto PageFilterOption::id() const -> const QString &
{
	return mOption.id();
}

auto PageFilterOption::label() const -> const QString &
{
	return mOption.label();
}
