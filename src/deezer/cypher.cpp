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
			const QByteArray encChunk = chunk.left(2048);
			const QByteArray decChunk = decryptChunk(key, iv, encChunk);

			result.append(decChunk);
			result.append(chunk.mid(2048));
		}
		else
		{
			result.append(chunk);
		}
	}

	return result;
}

auto Cypher::decryptChunk(const QByteArray &key,
	const std::array<quint8, 8> &iv, const QByteArray &data) -> QByteArray
{
	Blowfish blowfish(key, iv);

	auto *plaintext = new uint8_t[data.length()];

	{
		if (!blowfish.decrypt(data, plaintext, data.length()))
		{
			qCritical() << "Failed to decrypt data";
			return {};
		}
	}

	QByteArray result;
	result.reserve(data.length());
	result.append(reinterpret_cast<const char *>(plaintext), data.length());
	delete[] plaintext;

	return result;
}
