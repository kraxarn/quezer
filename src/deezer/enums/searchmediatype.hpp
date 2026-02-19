#pragma once

#include <QtTypes>

enum class SearchMediaType : quint8
{
	Album    = 1,
	Artist   = 2,
	Playlist = 3,
	Radio    = 4,
	Track    = 5,
	User     = 6,
};
