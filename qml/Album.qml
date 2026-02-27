import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
	property var margin: 6
	anchors {
		left: parent.left
		bottom: parent.bottom
		leftMargin: margin
		bottomMargin: margin
	}
	width: sidebar.width - (margin * 2)
	height: sidebar.width - (margin * 2)
	radius: 6
	color: parent.palette.window.lighter()

	Icon {
		name: "media-optical"
		size: 100
		anchors.centerIn: parent
	}
}
