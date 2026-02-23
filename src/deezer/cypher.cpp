#include "deezer/cypher.hpp"
#include "deezer/blowfish/blowfish.hpp"

#include <QCryptographicHash>

#include <numeric>
#include <QDebug>

static auto secret = QStringLiteral("g4el58wc0zvf9na1");

auto Cypher::generateKey(const qint64 seed) -> QByteArray
{
	const QByteArray hash = QCryptographicHash::hash(
		QString::number(seed).toUtf8(), QCryptographicHash::Md5
	).toHex();

	Q_ASSERT(hash.length() == 32);

	QByteArray result;
	result.reserve(16);

	for (auto i = 0; i < 16; i++)
	{
		result.append(hash.at(i) ^ hash.at(16 + i) ^ secret.at(i).toLatin1());
	}

	return result;
}

auto Cypher::decrypt(const QByteArray &key, const std::array<quint8, 8> &iv,
	const QByteArray &data) -> QByteArray
{
	if (data.isEmpty())
	{
		return {};
	}

	QByteArray result;
	result.reserve(data.length());

	constexpr qsizetype chunkSize = 2048 * 3;

	for (qsizetype i = 0; i < data.length(); i += chunkSize)
	{
		const QByteArray chunk = data.mid(i, chunkSize);

		if (chunk.length() >= 2048)
		{
			// We need a new instance to reset the state
			result.append(Blowfish(key, iv).decrypt(chunk.left(2048)));
			result.append(chunk.mid(2048));
		}
		else
		{
			result.append(chunk);
		}
	}

	return result;
}

