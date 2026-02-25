import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Pages.Login

Item {
	anchors.fill: parent

	ColumnLayout {
		id: loginLayout
		anchors {
			top: parent.top
			bottom: parent.bottom
			left: parent.left
			leftMargin: parent.width * 0.15
		}

		Login {
			id: page
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
			height: 32
		}

		Label {
			text: page.errorMessage
			color: "red"
			bottomPadding: 12
			visible: text.length > 0
		}

		Label {
			text: "Email"
		}

		TextField {
			id: email
			Layout.fillWidth: true
			onAccepted: login.click()
		}

		Item {
			height: 6
		}

		Label {
			text: "Password"
		}

		TextField {
			id: password
			Layout.fillWidth: true
			echoMode: TextInput.Password
			onAccepted: login.click()
		}

		Item {
			height: 16
		}

		Button {
			id: login
			Layout.fillWidth: true
			text: "Login"
			enabled: email.text.length > 0 && password.text.length > 0
			onPressed: page.login(email.text, password.text)
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
