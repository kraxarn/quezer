#include "deezer/objects/page.hpp"

#include <QJsonArray>

auto Page::fromJson(const QJsonObject &json) -> Page
{
	Page page;

	const QJsonArray sections = json
		.value(QStringLiteral("results")).toObject()
		.value(QStringLiteral("sections")).toArray();

	page.mSections.reserve(sections.size());
	for (const QJsonValueConstRef section: sections)
	{
		page.mSections.append(Section::fromJson(section.toObject()));
	}

	return page;
}

auto Page::Section::fromJson(const QJsonObject &json) -> Section
{
	Section section;

	const QJsonArray items = json
		.value(QStringLiteral("items")).toArray();

	section.mTitle = json.value(QStringLiteral("title")).toString();
	section.mSubtitle = json.value(QStringLiteral("subtitle")).toString();
	section.mLayout = json.value(QStringLiteral("layout")).toString();
	section.mTarget = json.value(QStringLiteral("target")).toString();

	section.mItems.reserve(items.size());
	for (const QJsonValueConstRef item: items)
	{
		section.mItems.append(Item::fromJson(item.toObject()));
	}

	section.mFilter = Filter::fromJson(
		json.value(QStringLiteral("filter")).toObject()
	);

	return section;
}

auto Page::Section::Item::fromJson(const QJsonObject &json) -> Item
{
	Item item;

	const QJsonArray pictures = json
		.value(QStringLiteral("pictures")).toArray();

	const QJsonArray filterOptionIds = json
		.value(QStringLiteral("filter_option_ids")).toArray();

	item.mTitle = json.value(QStringLiteral("title")).toString();
	item.mSubtitle = json.value(QStringLiteral("subtitle")).toString();

	item.mPictures.reserve(pictures.size());
	for (const QJsonValueConstRef picture: pictures)
	{
		item.mPictures.append(Picture::fromJson(picture.toObject()));
	}

	item.mFilterOptionIds.reserve(filterOptionIds.size());
	for (const QJsonValueConstRef optionId: filterOptionIds)
	{
		item.mFilterOptionIds.append(optionId.toString());
	}

	return item;
}

auto Page::Section::Item::Picture::fromJson(const QJsonObject &json) -> Picture
{
	Picture picture;

	picture.mMd5 = json.value(QStringLiteral("md5")).toString();
	picture.mType = json.value(QStringLiteral("type")).toString();

	return picture;
}

auto Page::Section::Filter::fromJson(const QJsonObject &json) -> Filter
{
	Filter filter;

	const QJsonArray options = json
		.value(QStringLiteral("options")).toArray();

	filter.mDefaultOptionId = json.value(QStringLiteral("default_option_id")).toString();

	filter.mOptions.reserve(options.size());
	for (const QJsonValueConstRef option: options)
	{
		filter.mOptions.append(Option::fromJson(option.toObject()));
	}

	return filter;
}

auto Page::Section::Filter::Option::fromJson(const QJsonObject &json) -> Option
{
	Option option;

	option.mId = json.value(QStringLiteral("id")).toString();
	option.mLabel = json.value(QStringLiteral("label")).toString();

	return option;
}

auto Page::Section::title() const -> const QString &
{
	return mTitle;
}

auto Page::Section::subtitle() const -> const QString &
{
	return mSubtitle;
}

auto Page::sections() const -> const QList<Section> &
{
	return mSections;
}
