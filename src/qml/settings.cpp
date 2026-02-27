#include "qml/settings.hpp"

#include <QDateTime>

Settings::Settings(QObject *parent)
	: QObject(parent),
	mSettings(this)
{
}

auto Settings::arl() const -> QString
{
	return mSettings.value("arl/value").toString();
}

void Settings::setArl(const QString &value)
{
	mSettings.setValue("arl/value", value);
	emit arlChanged();
}

void Settings::removeArl()
{
	mSettings.remove("arl/value");
}

auto Settings::arlExpiration() const -> QDateTime
{
	return mSettings.value("arl/expiration").toDateTime();
}

void Settings::setArlExpiration(const QDateTime &value)
{
	mSettings.setValue("arl/expiration", value);
	emit arlExpirationChanged();
}

void Settings::removeArlExpiration()
{
	mSettings.remove("arl/expiration");
}
