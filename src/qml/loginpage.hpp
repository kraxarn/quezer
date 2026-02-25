#pragma once

#include "deezer/enums/loginerror.hpp"

#include <QObject>
#include "qqmlintegration.h"

class LoginPage : public QObject
{
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

public:
	explicit LoginPage(QObject *parent = nullptr);

	[[nodiscard]]
	auto errorMessage() const -> const QString &;

	Q_INVOKABLE void login(const QString &email, const QString &password);

signals:
	void errorMessageChanged();
	void loggedIn();

private:
	QString mErrorMessage;

	void setErrorMessage(const QString &message);

	void onLoginFinished(LoginError error);
};
