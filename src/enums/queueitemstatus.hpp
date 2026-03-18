#pragma once

#include <QtTypes>

enum class QueueItemStatus: quint8
{
	/**
	 * Unknown status
	 */
	Unknown = 0,

	/**
	 * Loading song data
	 */
	Loading = 1,

	/**
	 * Loaded, ready for loading audio data
	 */
	Waiting = 2,

	/**
	 * Buffering audio data
	 */
	Buffering = 3,

	/**
	 * Ready to play
	 */
	Ready = 4,

	/**
	 * Something went wrong
	 */
	Error = 5,
};
