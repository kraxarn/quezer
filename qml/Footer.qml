import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
	anchors {
		left: parent.left
		right: parent.right
		bottom: parent.bottom
	}
	height: 80

	ColumnLayout {
		anchors {
			left: parent.left
			top: parent.top
			bottom: parent.bottom
			leftMargin: album.width + 14
		}

		Item {
			Layout.fillHeight: true
		}

		Label {
			id: trackTitle
			text: "Track title"
			font.pointSize: artistName.font.pointSize * 1.4
		}

		Label {
			id: artistName
			text: "Artist name"
		}

		Item {
			Layout.fillHeight: true
		}
	}

	Row {
		id: buttons

		anchors {
			top: parent.top
			horizontalCenter: progress.horizontalCenter
		}

		ToolButton {
			icon.source: "qrc:/mdi/shuffle-disabled.svg"
		}

		ToolButton {
			icon.source: "qrc:/mdi/skip-previous.svg"
		}

		ToolButton {
			icon.source: "qrc:/mdi/play.svg"
		}

		ToolButton {
			icon.source: "qrc:/mdi/skip-next.svg"
		}

		ToolButton {
			icon.source: "qrc:/mdi/repeat-off.svg"
		}
	}

	Slider {
		id: progress
		anchors {
			left: parent.left
			right: parent.right
			bottom: parent.bottom
			leftMargin: parent.width * 0.4
			rightMargin: parent.width * 0.4
		}
	}

	Row {
		anchors {
			top: parent.top
			bottom: parent.bottom
			right: parent.right
			rightMargin: 20
		}

		ToolButton {
			anchors {
				top: parent.top
				bottom: parent.bottom
			}
			icon.source: "qrc:/mdi/microphone-variant.svg"
		}

		ToolButton {
			anchors {
				top: parent.top
				bottom: parent.bottom
			}
			icon.source: "qrc:/mdi/format-list-numbered.svg"
		}

		Item {
			width: 20
			height: 1
		}

		ToolButton {
			anchors {
				top: parent.top
				bottom: parent.bottom
			}
			icon.source: "qrc:/mdi/volume-high.svg"
		}

		Slider {
			id: volume
			anchors {
				top: parent.top
				bottom: parent.bottom
			}
			from: 0
			to: 100
			value: 100
		}
	}
}
