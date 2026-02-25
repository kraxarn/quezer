#include "qml/loginpage.hpp"
#include "deezer/deezerclient.hpp"
#include "deezer/objects/options.hpp"

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
	if (error == LoginError::NoError)
	{
		ApiResponse *response = DeezerClient::instance()->api().options();
		connect(response, &ApiResponse::finished, [this, response]() -> void
		{
			if (!response->isValid())
			{
				setErrorMessage(response->errorString());
				response->deleteLater();
				return;
			}

			const Options options = response->value<Options>();
			response->deleteLater();

			if (options.adsDisplay())
			{
				setErrorMessage(QStringLiteral("A paid Deezer plan is required"));
				return;
			}

			if (!options.streaming())
			{
				setErrorMessage(QStringLiteral("Streaming is not supported in your region"));
				return;
			}

			emit loggedIn();
		});
		return;
	}

	switch (error)
	{
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

		default:
			break;
	}
}
