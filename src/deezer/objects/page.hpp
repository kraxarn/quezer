#pragma once

#include <QJsonObject>
#include <QList>

class Page final
{
public:
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
			[[nodiscard]]
			static auto fromJson(const QJsonObject &json) -> Item;

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
			Item() = default;

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
				[[nodiscard]]
				static auto fromJson(const QJsonObject &json) -> Option;

			private:
				Option() = default;

				QString mId;
				QString mLabel;
			};

		private:
			Filter() = default;

			QString mDefaultOptionId;
			QList<Option> mOptions;

			friend class Section;
		};

	private:
		Section() = default;

		QString mTitle;
		QString mSubtitle;
		QString mLayout;
		QString mTarget;
		QList<Item> mItems;
		Filter mFilter;
	};

private:
	Page() = default;

	QList<Section> mSections;
};
