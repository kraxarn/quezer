import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Models.HomePage
import Models.HomePageItem
import Proxies.HomePageItem
import ImagePaintedItem

ListView {
	anchors {
		top: parent.top
		bottom: parent.bottom
		bottomMargin: 50
	}
	model: HomePageModel {
	}
	delegate: Item {
		required property int itemHeight
		required property string title
		required property string subtitle
		required property var filterOption
		required property var filterOptions
		required property var items

		id: delegate
		height: itemHeight
		width: parent.width
		anchors {
			left: (parent || undefined) && parent.left
			right: (parent || undefined) && parent.right
			leftMargin: 20
			rightMargin: 20
		}

		Item {
			id: topPadding
			height: 70
		}

		Label {
			id: title
			anchors {
				top: topPadding.bottom
			}
			font {
				bold: true
				pointSize: 20
			}
			text: delegate.title
		}

		Label {
			id: subtitle
			anchors {
				top: title.bottom
			}
			font {
				pointSize: 14
			}
			opacity: 0.6
			visible: !!delegate.subtitle
			text: delegate.subtitle
		}

		ComboBox {
			id: filter
			visible: !!delegate.filterOption
			height: 45
			anchors {
				top: topPadding.bottom
				right: parent.right
			}
			textRole: "label"
			model: delegate.filterOptions
		}

		ListView {
			id: items
			visible: !delegate.items.isEmpty
			orientation: ListView.Horizontal
			spacing: 20
			height: parent.height * 0.75
			anchors {
				left: parent.left
				right: parent.right
				top: subtitle.visible ? subtitle.bottom : title.bottom
				topMargin: 20
			}
			model: HomePageItemProxy {
				sourceModel: itemModel
				filterOptionId: filter.currentIndex < 0
					? (delegate.filterOption && delegate.filterOption.id) || ""
					: delegate.filterOptions[filter.currentIndex].id
			}
			delegate: Column {
				required property string title
				required property string subtitle
				required property url pictureUrl
				required property int pictureSize
				required property int pictureRadius

				id: delegate
				width: pictureSize
				spacing: 5

				Rectangle {
					width: delegate.pictureSize
					height: delegate.pictureSize
					radius: delegate.pictureRadius
					color: parent.palette.window

					ImagePaintedItem {
						anchors.fill: parent
						source: delegate.pictureUrl
					}
				}

				Label {
					id: title
					anchors {
						left: parent.left
						right: parent.right
					}
					horizontalAlignment: Text.AlignHCenter
					wrapMode: Text.WordWrap
					text: delegate.title
				}

				Label {
					id: subtitle
					anchors {
						left: parent.left
						right: parent.right
					}
					font {
						pointSize: title.font.pointSize * 0.85
					}
					visible: delegate.subtitle.length > 0
						&& delegate.title !== delegate.subtitle
					horizontalAlignment: Text.AlignHCenter
					wrapMode: Text.WordWrap
					opacity: 0.6
					text: delegate.subtitle
				}
			}

			HomePageItemModel {
				id: itemModel
				items: delegate.items
			}
		}
	}
}
