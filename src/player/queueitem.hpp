#pragma once

#include "deezer/objects/songdata.hpp"
#include "enums/queueitemstatus.hpp"
#include "deezer/enums/mediaformat.hpp"

#include <QByteArray>

struct QueueItem final
{
	QueueItemStatus status;
	SongData songData;
	MediaFormat mediaFormat;
	QByteArray audioData;
};
