import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Layouts

import Pages.Player
import ImagePaintedItem

Item {
	Player {
		id: page
	}

	Rectangle {
		id: main
		anchors {
			left: sidebar.right
			right: parent.right
			top: parent.top
			bottom: footer.top
		}
		color: parent.palette.window.darker()

		RowLayout {
			property var margin: 10
			anchors {
				left: parent.left
				right: parent.right
				top: parent.top
				leftMargin: margin
				rightMargin: margin
				topMargin: margin
			}

			RoundButton {
				icon.name: "go-previous"
				enabled: false
			}

			RoundButton {
				Layout.leftMargin: parent.margin * 0.5
				icon.name: "system-search"
			}

			Item {
				Layout.fillWidth: true
			}

			RoundButton {
				icon.name: "mail-mark-read" // TODO
			}

			RoundButton {
				icon.name: "user-offline"
				contentItem: ImagePaintedItem {
					image: page.userImage
				}
			}
		}
	}

	Footer {
		id: footer
	}

	Sidebar {
		id: sidebar
	}

	Album {
		id: album
	}
}
