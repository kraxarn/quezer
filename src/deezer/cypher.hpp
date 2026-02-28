#pragma once

#include "deezer/blowfish/blowfish.hpp"

#include <QString>

class Cypher
{
public:
	[[nodiscard]]
	static auto generateKey(qint64 seed) -> QByteArray;

	[[nodiscard]]
	static auto generateIv() -> IV;

	[[nodiscard]]
	static auto decrypt(const QByteArray &key, const IV &iv,
		const QByteArray &data) -> QByteArray;
};
