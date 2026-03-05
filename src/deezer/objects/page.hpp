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

	private:
		Section() = default;

		QString mTitle;
		QString mSubtitle;
		QString mLayout;
		QString mTarget;
		QList<Item> mItems;
	};

private:
	Page() = default;

	QList<Section> mSections;
};
