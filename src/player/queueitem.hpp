#pragma once

#include "deezer/enums/mediaformat.hpp"
#include "enums/queueitemstatus.hpp"

#include <QByteArray>
#include <QtTypes>
#include <QUrl>

struct QueueItem final
{
	qint64 trackId;
	MediaFormat mediaFormat;
	QUrl mediaUrl;
	QByteArray audioData;
	QueueItemStatus status;
};
