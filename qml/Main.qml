import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 1280
    height: 720
    visible: true
    title: `${AppName}`

	Column {
		Label {
			text: `${AppName} ${AppVersion}`
		}

		Label {
			text: `Qt ${QtVersion}`
		}

		Label {
			text: `${BuildDate}`
		}
	}
}
