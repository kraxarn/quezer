import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
	Rectangle {
		id: main
		anchors {
			left: sidebar.right
			right: parent.right
			top: parent.top
			bottom: footer.top
		}
		color: parent.palette.window.darker()
	}

	Footer {
		id: footer
	}

	Sidebar {
		id: sidebar
	}

	Album {
		id: album
	}
}
