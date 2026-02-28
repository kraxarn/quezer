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
				source: model.action || `qrc:/mdi/${model.icon}.svg`
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
				icon.source: `qrc:/mdi/${model.icon}.svg`
				visible: model.action
			}
		}
	}
}
