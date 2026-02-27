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

	RowLayout {
		id: buttons

		anchors {
			left: progress.left
			right: progress.right
			top: parent.top
		}

		Item {
			Layout.fillWidth: true
		}

		ToolButton {
			icon.name: "media-playlist-shuffle"
		}

		ToolButton {
			icon.name: "media-skip-backward"
		}

		ToolButton {
			icon.name: "media-playback-start"
		}

		ToolButton {
			icon.name: "media-skip-forward"
		}

		ToolButton {
			icon.name: "media-playlist-repeat"
		}

		Item {
			Layout.fillWidth: true
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
			icon.name: "audio-input-microphone"
		}

		ToolButton {
			anchors {
				top: parent.top
				bottom: parent.bottom
			}
			icon.name: "format-justify-fill" // TODO
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
			icon.name: "audio-volume-high"
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
