#pragma once

#include <QtTypes>

enum class LoginError: quint8
{
	NoError            = 0,
	NoUserData         = 1,
	NoCheckFormLogin   = 2,
	NetworkError       = 3,
	InvalidCredentials = 4,
	InvalidCookie      = 5,
	NoArl              = 6,
};
