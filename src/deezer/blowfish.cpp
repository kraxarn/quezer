#include "deezer/blowfish.hpp"
#include "deezer/blowfish/blowfishcbc.h"

#include <QCryptographicHash>

#include <numeric>
#include <QDebug>

static auto secret = QStringLiteral("g4el58wc0zvf9na1");

auto Crypto::generateKey(const qint64 seed) -> QByteArray
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

auto Crypto::decrypt(const QByteArray &key, const QByteArray &iv, const QByteArray &data) -> QByteArray
{
	if (data.isEmpty())
	{
		return QByteArray();
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

auto Crypto::decryptChunk(const QByteArray &key,
	const QByteArray &iv, const QByteArray &data) -> QByteArray
{
	Blowfish_State *cypherState = nullptr;
	{
		const int result = Blowfish_start_operation(
			reinterpret_cast<const uint8_t *>(key.constData()),
			key.length(), &cypherState
		);

		if (result != 0)
		{
			qCritical() << "Failed to initiate blowfish: error" << result;
			return {};
		}
	}

	if (iv.length() != 8)
	{
		qCritical() << "Invalid IV length (must be 8 bytes)";
		return {};
	}

	CbcModeState *cbcState = nullptr;
	{
		const int result = CBC_start_operation(
			reinterpret_cast<BlockBase *>(cypherState),
			reinterpret_cast<const uint8_t *>(iv.constData()),
			iv.length(), // block_size
			&cbcState
		);

		if (result != 0)
		{
			qCritical() << "Failed to initiate CBC mode: error" << result;
			return {};
		}
	}

	auto *plaintext = new uint8_t[data.length()];

	{
		const int result = CBC_decrypt(cbcState,
			reinterpret_cast<const uint8_t *>(data.data()),
			plaintext,data.length()
		);

		if (result == 3)
		{
			qCritical() << "Data must be padded to 8 byte boundry";
			return {};
		}

		if (result != 0)
		{
			qCritical() << "Failed to decrypt data: error" << result;
			return {};
		}
	}

	CBC_stop_operation(cbcState);

	QByteArray result;
	result.reserve(data.length());
	result.append(reinterpret_cast<const char *>(plaintext), data.length());
	delete[] plaintext;

	return result;
}
