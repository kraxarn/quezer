#pragma once

#include <QtTypes>

enum class ExplicitContent : quint8
{
	NotExplicit                = 0,
	Explicit                   = 1,
	Unknown                    = 2,
	Edited                     = 3,
	PartiallyExplicit          = 4,
	PartiallyUnknown           = 5,
	NoAdviceAvailable          = 6,
	PartiallyNoAdviceAvailable = 7,
};
