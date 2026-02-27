import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
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
				icon.name: "user-offline"
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
