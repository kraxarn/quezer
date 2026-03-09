#pragma once

#include "deezer/objects/page.hpp"

#include <QObject>
#include <qqmlintegration.h>

class PageFilterOption : public QObject
{
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(QString id READ id NOTIFY idChanged)
	Q_PROPERTY(QString label READ label NOTIFY labelChanged)

public:
	explicit PageFilterOption(QObject *parent = nullptr);

	explicit PageFilterOption(const Page::Section::Filter::Option &option, QObject *parent = nullptr);

	[[nodiscard]]
	auto id() const -> const QString &;

	[[nodiscard]]
	auto label() const -> const QString &;

signals:
	void idChanged();

	void labelChanged();

private:
	Page::Section::Filter::Option mOption;
};

Q_DECLARE_METATYPE(PageFilterOption);
