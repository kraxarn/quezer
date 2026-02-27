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
		delegate: ItemDelegate {
			height: 40
			width: parent.width
			text: model.text
			highlighted: ListView.isCurrentItem
			icon {
				name: model.icon
			}
			font {
				bold: true
				pointSize: 14
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
