#pragma once

#include <QtTypes>

enum class MediaFormat: quint8
{
	/**
	 * MP3, 128 KB/s
	 */
	LowQuality,

	/**
	 * MP3, 320 KB/s
	 */
	HighQuality,

	/**
	 * FLAC
	 */
	Lossless,
};
