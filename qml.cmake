# QML files are in qml/
if (QT_KNOWN_POLICY_QTP0004)
	qt_policy(SET QTP0004 NEW)
endif ()

qt_add_qml_module(${APP_NAME}
	URI "kraxarn.${APP_NAME}"
	VERSION "${PROJECT_VERSION}"
	NO_RESOURCE_TARGET_PATH

	QML_FILES
	qml/Login.qml
	qml/Main.qml
	qml/Player.qml
	qml/PlayerFooter.qml

	RESOURCES
	res/logo/quezer.svg
)
