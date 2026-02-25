#include "qml/loginpage.hpp"

#include <QtDebug>

LoginPage::LoginPage(QObject *parent)
	: QObject(parent)
{
}

auto LoginPage::errorMessage() const -> QString
{
	return {};
}

void LoginPage::login(const QString &email, const QString &password)
{
	qDebug() << "email:" << email << "password:" << password;
}
