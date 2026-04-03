#pragma once

#include <QJsonArray>
#include <QStringList>

class Json final
{
public:
	Json() = delete;

	[[nodiscard]]
	static auto stringList(const QJsonArray &json) -> QStringList;
};
