import QtQuick
import QtQuick.Controls

Item {
	property string name
	property int size

	width: size
	height: size

	AbstractButton {
		anchors.fill: parent
		z: 1
	}

	Button {
		anchors.fill: parent
		flat: true
		checkable: false
		padding: 0
		icon {
			name: parent.name
			width: parent.size
			height: parent.size
		}
	}
}
