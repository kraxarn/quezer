import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
	id: root
	width: 1280
	height: 720
	visible: true
	title: `${AppName}`

	ColumnLayout {
		id: loginLayout
		anchors {
			top: parent.top
			bottom: parent.bottom
			left: parent.left
			leftMargin: parent.width * 0.15
		}

		Item {
			Layout.fillHeight: true
		}

		Label {
			font {
				bold: true
				pointSize: 28
			}
			text: "Log in to Deezer"
		}

		Item {
			height: root.height * 0.075
		}

		Label {
			text: "Username"
		}

		TextField {
			Layout.fillWidth: true
		}

		Item {
			height: 6
		}

		Label {
			text: "Password"
		}

		TextField {
			Layout.fillWidth: true
		}

		Item {
			height: 16
		}

		Button {
			Layout.fillWidth: true
			text: "Login"
		}

		Item {
			Layout.fillHeight: true
		}
	}

	Item {
		id: logo
		anchors {
			top: parent.top
			bottom: parent.bottom
			left: loginLayout.right
			right: parent.right
		}

		Image {
			anchors.centerIn: parent
			sourceSize: Qt.size(256, 256)
			source: "qrc:/res/logo/quezer.svg"
		}
	}
}
