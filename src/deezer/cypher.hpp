#pragma once

#include <QString>

class Cypher
{
public:
	[[nodiscard]]
	static auto generateKey(qint64 seed) -> QByteArray;

	[[nodiscard]]
	static auto decrypt(const QByteArray &key, const QByteArray &iv, const QByteArray &data) -> QByteArray;

	[[nodiscard]]
	static auto decryptChunk(const QByteArray &key,
		const QByteArray &iv, const QByteArray &data) -> QByteArray;
};
