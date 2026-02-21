#include "deezer/blowfish/blowfish.hpp"
#include "deezer/blowfish/constants.hpp"

#include <QtDebug>
#include <QtEndian>

// Based of:
// https://github.com/Legrandin/pycryptodome

constexpr quint8 blockSize = 8;

Blowfish::Blowfish(const QByteArray &key, const IV &iv)
	: iv(iv)
{
	if (!initBlock(key))
	{
		return;
	}
}

auto Blowfish::f(const quint32 x) const -> quint32
{
	const auto a = static_cast<uint8_t>(x >> 24);
	const auto b = static_cast<uint8_t>(x >> 16);
	const auto c = static_cast<uint8_t>(x >> 8);
	const auto d = static_cast<uint8_t>(x >> 0);

	quint32 result = S[0][a] + S[1][b];
	result ^= S[2][c];
	result += S[3][d];

	return result;
}

void Blowfish::encrypt(quint32 *Lx, quint32 *Rx) const
{
	quint32 L = *Lx;
	quint32 R = *Rx;

	for (qsizetype i = 0; i < 16; i++)
	{
		L ^= P[i];
		R ^= f(L);
		std::swap(L, R);
	}

	std::swap(L, R);
	R ^= P[16];
	L ^= P[17];

	*Lx = L;
	*Rx = R;
}

void Blowfish::decrypt(quint32 *Lx, quint32 *Rx) const
{
	quint32 L = *Lx;
	quint32 R = *Rx;

	L ^= P[17];
	R ^= P[16];
	std::swap(L, R);

	for (qsizetype i = 0; i < 16; i++)
	{
		std::swap(L, R);
		R ^= f(L);
		L ^= P[15 - i];
	}

	*Lx = L;
	*Rx = R;
}

void Blowfish::xorP(const QByteArray &key)
{
	std::array<uint8_t, 4 * 18> P_buf;

	size_t P_idx = 0;
	while (P_idx < P_buf.size())
	{
		const qsizetype tc = qMin(key.length(), static_cast<qsizetype>(sizeof(P_buf) - P_idx));
		memcpy(P_buf.data() + P_idx, key.constData(), tc);

		P_idx += tc;
	}

	P_idx = 0;
	for (qsizetype i = 0; i < 18; i++)
	{
		P.at(i) ^= qToBigEndian(*reinterpret_cast<uint32_t *>(P_buf.data() + P_idx));
		P_idx += 4;
	}
}

void Blowfish::encrypt(const QByteArray &key)
{
	xorP(key);

	quint32 L = 0;
	quint32 R = 0;

	for (qsizetype i = 0; i < 18; i += 2)
	{
		encrypt(&L, &R);
		P[i] = L;
		P[i + 1] = R;
	}
	for (qsizetype j = 0; j < 4; j++)
	{
		for (qsizetype i = 0; i < 256; i += 2)
		{
			encrypt(&L, &R);
			S[j][i] = L;
			S[j][i + 1] = R;
		}
	}
}

auto Blowfish::initBlock(const QByteArray &key) -> bool
{
	if (key.length() < 4 || key.length() > 56)
	{
		return false;
	}

	S = S_init;
	P = P_init;

	encrypt(key);

	return true;
}

void Blowfish::decrypt(const uint8_t *in, uint8_t *out) const
{
	quint32 lx = qToBigEndian(*reinterpret_cast<const uint32_t *>(in));
	quint32 rx = qToBigEndian(*reinterpret_cast<const uint32_t *>(in + 4));

	decrypt(&lx, &rx);

	*reinterpret_cast<quint32 *>(out) = qFromBigEndian(lx);
	*reinterpret_cast<quint32 *>(out + 4) = qFromBigEndian(rx);
}

auto Blowfish::decrypt(const uint8_t *in, uint8_t *out, size_t length) -> bool
{
	std::array<uint8_t, blockSize> tempPt;
	std::array<uint8_t, blockSize> tempIv;

	if (in == nullptr || out == nullptr)
	{
		return false;
	}

	tempIv = iv;

	while (length >= blockSize)
	{
		decrypt(in, tempPt.data());

		for (qsizetype i = 0; i < blockSize; i++)
		{
			tempPt.at(i) = tempPt.at(i) ^ tempIv.at(i);
		}

		memcpy(tempIv.data(), in, blockSize);
		memcpy(out, tempPt.data(), blockSize);

		length -= blockSize;
		in += blockSize;
		out += blockSize;
	}

	iv = tempIv;

	return length <= 0;
}
