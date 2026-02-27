import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
	anchors {
		left: parent.left
		top: parent.top
		bottom: parent.bottom
	}
	width: 240

	ListView {
		anchors.fill: parent

		model: ListModel {
			ListElement {
				icon: "go-home"
				text: "Home"
			}

			ListElement {
				icon: "audio-card" // TODO
				text: "Explore"
			}

			ListElement {
				icon: "mail-mark-important" // TODO
				text: "Favourites"
			}

			ListElement {
				text: "Playlists"
			}
		}
		delegate: Item {
			height: 40
			width: parent.width

			Icon {
				id: icon
				anchors {
					left: parent.left
					verticalCenter: parent.verticalCenter
				}
				name: model.icon
				size: model.icon ? 45 : 0
			}

			Label {
				id: label
				anchors {
					left: icon.right
					leftMargin: model.icon ? 0 : 8
					verticalCenter: parent.verticalCenter
				}
				font {
					pointSize: 14
					bold: true
				}
				text: model.text
			}

			ToolButton {
				id: action
				anchors {
					right: parent.right
					verticalCenter: parent.verticalCenter
				}
				icon.name: "document-new"
				visible: !model.icon
			}
		}
	}
}
