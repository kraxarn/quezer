#include "deezer/blowfish/blowfish.hpp"
#include "deezer/blowfish/constants.hpp"

#include <QtDebug>
#include <QtEndian>

// Based of:
// https://github.com/Legrandin/pycryptodome

constexpr quint8 blockSize = 8;

Blowfish::Blowfish(const QByteArray &key, const IV &iv)
	: mIv(iv)
{
	if (!initBlock(key))
	{
		return;
	}
}

auto Blowfish::f(const quint32 x) const -> quint32
{
	const auto a = static_cast<quint8>(x >> 24);
	const auto b = static_cast<quint8>(x >> 16);
	const auto c = static_cast<quint8>(x >> 8);
	const auto d = static_cast<quint8>(x >> 0);

	quint32 result = mS[0][a] + mS[1][b];
	result ^= mS[2][c];
	result += mS[3][d];

	return result;
}

void Blowfish::encrypt(quint32 *lx, quint32 *rx) const
{
	quint32 l = *lx;
	quint32 r = *rx;

	for (qsizetype i = 0; i < 16; i++)
	{
		l ^= mP[i];
		r ^= f(l);
		std::swap(l, r);
	}

	std::swap(l, r);
	r ^= mP[16];
	l ^= mP[17];

	*lx = l;
	*rx = r;
}

void Blowfish::decrypt(quint32 *lx, quint32 *rx) const
{
	quint32 l = *lx;
	quint32 r = *rx;

	l ^= mP[17];
	r ^= mP[16];
	std::swap(l, r);

	for (qsizetype i = 0; i < 16; i++)
	{
		std::swap(l, r);
		r ^= f(l);
		l ^= mP[15 - i];
	}

	*lx = l;
	*rx = r;
}

void Blowfish::xorP(const QByteArray &key)
{
	std::array<quint8, 4 * 18> pBuf;

	size_t pIdx = 0;
	while (pIdx < pBuf.size())
	{
		const qsizetype tc = qMin(key.length(), static_cast<qsizetype>(sizeof(pBuf) - pIdx));
		memcpy(pBuf.data() + pIdx, key.constData(), tc);

		pIdx += tc;
	}

	pIdx = 0;
	for (qsizetype i = 0; i < 18; i++)
	{
		mP.at(i) ^= qToBigEndian(*reinterpret_cast<quint32 *>(pBuf.data() + pIdx));
		pIdx += 4;
	}
}

void Blowfish::encrypt(const QByteArray &key)
{
	xorP(key);

	quint32 l = 0;
	quint32 r = 0;

	for (qsizetype i = 0; i < 18; i += 2)
	{
		encrypt(&l, &r);
		mP[i] = l;
		mP[i + 1] = r;
	}

	for (qsizetype j = 0; j < 4; j++)
	{
		for (qsizetype i = 0; i < 256; i += 2)
		{
			encrypt(&l, &r);
			mS[j][i] = l;
			mS[j][i + 1] = r;
		}
	}
}

auto Blowfish::initBlock(const QByteArray &key) -> bool
{
	if (key.length() < 4 || key.length() > 56)
	{
		return false;
	}

	mS = S_init;
	mP = P_init;

	encrypt(key);

	return true;
}

void Blowfish::decrypt(const quint8 *in, quint8 *out) const
{
	quint32 lx = qToBigEndian(*reinterpret_cast<const quint32 *>(in));
	quint32 rx = qToBigEndian(*reinterpret_cast<const quint32 *>(in + 4));

	decrypt(&lx, &rx);

	*reinterpret_cast<quint32 *>(out) = qFromBigEndian(lx);
	*reinterpret_cast<quint32 *>(out + 4) = qFromBigEndian(rx);
}

auto Blowfish::decrypt(const QByteArray &data) -> QByteArray
{
	std::array<quint8, blockSize> tempPt;
	std::array<quint8, blockSize> tempIv;

	QByteArray result;
	result.reserve(data.length());

	auto current = reinterpret_cast<const quint8 *>(data.constData());
	qsizetype length = data.length();

	tempIv = mIv;

	while (length >= blockSize)
	{
		decrypt(current, tempPt.data());

		for (qsizetype i = 0; i < blockSize; i++)
		{
			tempPt.at(i) = tempPt.at(i) ^ tempIv.at(i);
		}

		memcpy(tempIv.data(), current, blockSize);
		result.append(reinterpret_cast<const char *>(tempPt.data()), blockSize);

		length -= blockSize;
		current += blockSize;
	}

	mIv = tempIv;

	if (length > 0)
	{
		return {};
	}

	return result;
}
