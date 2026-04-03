import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ImagePaintedItem

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

	ImagePaintedItem {
		id: albumPicture
		anchors {
			fill: parent
		}
		visible: !!image

		Connections {
			target: page.metaData

			function onAlbumPictureChanged() {
				albumPicture.image = page.metaData.albumPicture
			}
		}
	}

	Icon {
		name: "media-optical"
		size: 100
		anchors.centerIn: parent
		visible: !page.metaData || !page.metaData.albumPicture
	}
}
