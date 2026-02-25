#include "qml/loginpage.hpp"
#include "deezer/deezerclient.hpp"

LoginPage::LoginPage(QObject *parent)
	: QObject(parent)
{
	const DeezerClient *client = DeezerClient::instance();

	connect(client, &DeezerClient::loginFinished,
		this, &LoginPage::onLoginFinished);
}

auto LoginPage::errorMessage() const -> const QString &
{
	return mErrorMessage;
}

void LoginPage::setErrorMessage(const QString &message)
{
	mErrorMessage = message;
	emit errorMessageChanged();
}

void LoginPage::login(const QString &email, const QString &password)
{
	setErrorMessage(QString());
	DeezerClient::instance()->login(email, password);
}

void LoginPage::onLoginFinished(const LoginError error)
{
	switch (error)
	{
		case LoginError::NoError:
			setErrorMessage(QStringLiteral("Login successful"));
			break;

		case LoginError::NoUserData:
			setErrorMessage(QStringLiteral("Request failed: No user data"));
			break;

		case LoginError::NoCheckFormLogin:
			setErrorMessage(QStringLiteral("Request failed: No checkFormLogin value"));
			break;

		case LoginError::NetworkError:
			setErrorMessage(QStringLiteral("Request failed: Network error"));
			break;

		case LoginError::InvalidCredentials:
			setErrorMessage(QStringLiteral("Invalid username or password"));
			break;

		case LoginError::InvalidCookie:
			setErrorMessage(QStringLiteral("Request failed: Invalid cookie"));
			break;

		case LoginError::NoArl:
			setErrorMessage(QStringLiteral("Request failed: No ARL cookie"));
			break;
	}
}
