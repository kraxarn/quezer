#pragma once

#include <qqmlintegration.h>
#include <QSettings>

class Settings : public QObject
{
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(QString arl READ arl NOTIFY arlChanged)
	Q_PROPERTY(QDateTime arlExpiration READ arlExpiration NOTIFY arlExpirationChanged)

public:
	explicit Settings(QObject *parent);

	[[nodiscard]]
	auto arl() const -> QString;

	void setArl(const QString &value);

	[[nodiscard]]
	auto arlExpiration() const -> QDateTime;

	void setArlExpiration(const QDateTime &value);

signals:
	void arlChanged();

	void arlExpirationChanged();

private:
	QSettings mSettings;
};
