import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
	id: root
	width: 1280
	height: 720
	visible: true
	title: `${AppName}`

	StackView {
		id: content
		anchors.fill: parent
		initialItem: loginPage
	}

	Component {
		id: loginPage
		Login {
		}
	}
}
