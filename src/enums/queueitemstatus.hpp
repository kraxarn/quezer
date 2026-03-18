#pragma once

#include <QtTypes>

enum class QueueItemStatus: quint8
{
	Unknown   = 0,
	Waiting   = 1,
	Buffering = 2,
	Ready     = 3,
	Error     = 4,
};
