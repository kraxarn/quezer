#include "deezer/objects/mediaurl.hpp"

#include <QJsonArray>
#include <QJsonObject>

auto MediaUrl::fromJson(const QJsonObject &json) -> MediaUrl
{
	MediaUrl result;

	const QJsonObject media = json
		.value(QStringLiteral("data")).toArray().at(0).toObject()
		.value(QStringLiteral("media")).toArray().at(0).toObject();

	const QJsonArray sources = media
		.value(QStringLiteral("sources")).toArray();

	result.mSources.reserve(sources.size());
	for (const QJsonValueConstRef value: sources)
	{
		result.mSources.append(Source::fromJson(value.toObject()));
	}

	return result;
}

auto MediaUrl::Source::fromJson(const QJsonObject &json) -> Source
{
	Source result;

	result.mProvider = json.value(QStringLiteral("provider")).toString();
	result.mUrl = json.value(QStringLiteral("url")).toString();

	return result;
}

auto MediaUrl::Source::provider() const -> const QString &
{
	return mProvider;
}

auto MediaUrl::Source::url() const -> const QUrl &
{
	return mUrl;
}

auto MediaUrl::sources() const -> const QList<Source> &
{
	return mSources;
}
