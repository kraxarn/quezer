import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Models.Sidebar

Item {
	anchors {
		left: parent.left
		top: parent.top
		bottom: parent.bottom
	}
	width: 240

	ListView {
		anchors.fill: parent

		model: SidebarModel {
		}

		delegate: ItemDelegate {
			height: 40
			width: parent.width
			text: model.text
			highlighted: ListView.isCurrentItem
			icon {
				name: model.action || model.icon
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
				icon.name: model.icon
				visible: model.action
			}
		}
	}
}
