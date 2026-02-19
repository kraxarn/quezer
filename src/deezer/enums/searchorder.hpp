#pragma once

#include <QtTypes>

enum class SearchOrder : quint8
{
	Ranking      = 1,
	TrackAsc     = 2,
	TrackDesc    = 3,
	ArtistAsc    = 4,
	ArtistDesc   = 5,
	AlbumAsc     = 6,
	AlbumDesc    = 7,
	RatingAsc    = 8,
	RatingDesc   = 9,
	DurationAsc  = 10,
	DurationDesc = 11,
};
