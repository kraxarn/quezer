#pragma once

#include <QByteArray>
#include <QtTypes>

#include <array>

using IV = std::array<quint8, 8>;

class Blowfish final
{
public:
	Blowfish(const QByteArray &key, const IV &iv);

	[[nodiscard]]
	auto decrypt(const QByteArray &data) -> QByteArray;

private:
	std::array<std::array<quint32, 256>, 4> mS;
	std::array<quint32, 18> mP;
	IV mIv;

	[[nodiscard]]
	auto f(quint32 x) const -> quint32;

	void encrypt(quint32 *lx, quint32 *rx) const;

	void decrypt(quint32 *lx, quint32 *rx) const;

	void xorP(const QByteArray &key);

	void encrypt(const QByteArray &key);

	auto initBlock(const QByteArray &key) -> bool;

	void decrypt(const uint8_t *in, uint8_t *out) const;
};
