import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Models.HomePage

ListView {
	model: HomePageModel {
	}
	delegate: Item {
		required property string title
		required property string subtitle
		required property var filterOption
		required property var filterOptions

		id: delegate
		height: 100
		width: parent.width
		anchors {
			left: parent.left
			right: parent.right
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
			visible: !!delegate.filterOption
			height: 45
			anchors {
				top: topPadding.bottom
				right: parent.right
			}
			textRole: "label"
			model: delegate.filterOptions
		}
	}
}
