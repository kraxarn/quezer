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
	static auto fromJson(const QJsonDocument &json) -> Page
	{
		Page result;

		const QJsonObject obj = json.object();
		const QJsonArray data = obj.value(QStringLiteral("data")).toArray();

		result.mTotal = obj.value(QStringLiteral("total")).toInt();
		result.mNext = obj.value(QStringLiteral("next")).toString();

		result.mData.reserve(data.size());
		for (const QJsonValueConstRef value: data)
		{
			// TODO: Might have to re-think this
			result.mData.append(T::fromJson(QJsonDocument(value.toObject())));
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
	Page() = default;

	QList<T> mData;
	qint32 mTotal;
	QUrl mNext;
};
