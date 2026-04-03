#include "util/json.hpp"

auto Json::stringList(const QJsonArray &json) -> QStringList
{
	QStringList list;
	list.reserve(json.size());

	for (const QJsonValueConstRef value: json)
	{
		list.append(value.toString());
	}

	return list;
}
