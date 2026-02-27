import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Settings

ApplicationWindow {
	id: root
	width: 1280
	height: 720
	visible: true
	title: `${AppName}`

	Material.theme: Material.Dark
	Material.accent: "#a238ff"

	Settings {
		id: settings
	}

	StackView {
		id: content
		anchors.fill: parent
		initialItem: loginPage

		Component.onCompleted: {
			if (settings.arl.length > 0) {
				content.replace(playerPage)
			}
		}
	}

	Component {
		id: loginPage
		Login {
		}
	}

	Component {
		id: playerPage
		Player {
		}
	}
}
