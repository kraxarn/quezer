#pragma once

#include <QJsonObject>
#include <QList>

class Page final
{
public:
	Page() = default;

	[[nodiscard]]
	static auto fromJson(const QJsonObject &json) -> Page;

	class Section final
	{
	public:
		[[nodiscard]]
		static auto fromJson(const QJsonObject &json) -> Section;

		class Item final
		{
		public:
			Item() = default;

			[[nodiscard]]
			static auto fromJson(const QJsonObject &json) -> Item;

			[[nodiscard]]
			auto title() const -> const QString &;

			[[nodiscard]]
			auto subtitle() const -> const QString &;

			class Picture final
			{
			public:
				[[nodiscard]]
				static auto fromJson(const QJsonObject &json) -> Picture;

			private:
				QString mMd5;
				QString mType;
			};

		private:
			QString mTitle;
			QString mSubtitle;
			QList<Picture> mPictures;
			QStringList mFilterOptionIds;
		};

		class Filter final
		{
		public:
			[[nodiscard]]
			static auto fromJson(const QJsonObject &json) -> Filter;

			class Option final
			{
			public:
				Option() = default;

				[[nodiscard]]
				static auto fromJson(const QJsonObject &json) -> Option;

				[[nodiscard]]
				auto id() const -> const QString &;

				[[nodiscard]]
				auto label() const -> const QString &;

			private:
				QString mId;
				QString mLabel;
			};

			[[nodiscard]]
			auto defaultOptionId() const -> const QString &;

			[[nodiscard]]
			auto options() const -> const QList<Option> &;

		private:
			Filter() = default;

			QString mDefaultOptionId;
			QList<Option> mOptions;

			friend class Section;
		};

		[[nodiscard]]
		auto title() const -> const QString &;

		[[nodiscard]]
		auto subtitle() const -> const QString &;

		[[nodiscard]]
		auto items() const -> const QList<Item> &;

		[[nodiscard]]
		auto filter() const -> const Filter &;

	private:
		Section() = default;

		QString mTitle;
		QString mSubtitle;
		QString mLayout;
		QString mTarget;
		QList<Item> mItems;
		Filter mFilter;
	};

	[[nodiscard]]
	auto sections() const -> const QList<Section> &;

private:
	QList<Section> mSections;
};
