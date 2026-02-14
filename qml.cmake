# QML files are in qml/
if (QT_KNOWN_POLICY_QTP0004)
	qt_policy(SET QTP0004 NEW)
endif ()

qt_add_qml_module(${APP_NAME}
	URI ${PROJECT_NAME}
	VERSION ${PROJECT_VERSION}
	QML_FILES
	qml/Main.qml
)
