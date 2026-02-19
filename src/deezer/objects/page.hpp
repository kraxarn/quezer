#pragma once

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QUrl>

template<typename T>
class Page
{
public:
	Page() = default;

	[[nodiscard]]
	static auto fromJson(const QJsonObject &json) -> Page
	{
		Page result;

		const QJsonArray data = json.value(QStringLiteral("data")).toArray();

		result.mTotal = json.value(QStringLiteral("total")).toInt();
		result.mNext = json.value(QStringLiteral("next")).toString();

		result.mData.reserve(data.size());
		for (const QJsonValueConstRef value: data)
		{
			result.mData.append(T::fromJson(value.toObject()));
		}

		return result;
	}

	[[nodiscard]]
	auto data() const -> const QList<T> &
	{
		return mData;
	}

	[[nodiscard]]
	auto total() const -> int
	{
		return mTotal;
	}

	[[nodiscard]]
	auto next() const -> const QUrl &
	{
		return mNext;
	}

private:
	QList<T> mData;
	qint32 mTotal;
	QUrl mNext;
};
