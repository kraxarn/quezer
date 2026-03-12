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
		clip: true
		anchors {
			left: sidebar.right
			right: parent.right
			top: parent.top
			bottom: footer.top
		}
		color: parent.palette.window.darker()

		RowLayout {
			property var margin: 10
			z: 10
			anchors {
				left: parent.left
				right: parent.right
				top: parent.top
				leftMargin: margin
				rightMargin: margin
				topMargin: margin
			}

			RoundButton {
				icon.source: "qrc:/mdi/chevron-left.svg"
				enabled: false
			}

			RoundButton {
				Layout.leftMargin: parent.margin * 0.5
				icon.source: "qrc:/mdi/magnify.svg"
			}

			Item {
				Layout.fillWidth: true
			}

			RoundButton {
				icon.source: "qrc:/mdi/bell-outline.svg"
			}

			RoundButton {
				icon.source: "qrc:/mdi/account-circle.svg"
				contentItem: ImagePaintedItem {
					image: page.userImage
				}
			}
		}

		StackView {
			id: mainContent
			anchors.fill: parent
			initialItem: home
		}

		Component {
			id: home
			Home {
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
