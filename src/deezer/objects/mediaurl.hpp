#pragma once

#include <QObject>
#include <QUrl>

class MediaUrl final
{
public:
	[[nodiscard]]
	static auto fromJson(const QJsonObject &json) -> MediaUrl;

	class Source final
	{
	public:
		[[nodiscard]]
		static auto fromJson(const QJsonObject &json) -> Source;

		[[nodiscard]]
		auto provider() const -> const QString &;

		[[nodiscard]]
		auto url() const -> const QUrl &;

	private:
		Source() = default;

		QString mProvider;
		QUrl mUrl;
	};

	[[nodiscard]]
	auto sources() const -> const QList<Source> &;

private:
	MediaUrl() = default;

	QList<Source> mSources;
};
