# QML files are in qml/
if (QT_KNOWN_POLICY_QTP0004)
	qt_policy(SET QTP0004 NEW)
endif ()

qt_add_qml_module(${APP_NAME}
	URI "kraxarn.${APP_NAME}"
	VERSION "${PROJECT_VERSION}"
	NO_RESOURCE_TARGET_PATH

	QML_FILES
	qml/Album.qml
	qml/Footer.qml
	qml/Icon.qml
	qml/Login.qml
	qml/Main.qml
	qml/Player.qml
	qml/Sidebar.qml

	RESOURCES
	res/logo/quezer.svg
)

set(MDI_DIR "${mdi_SOURCE_DIR}/svg")
file(RELATIVE_PATH MDI_DIR "${CMAKE_CURRENT_SOURCE_DIR}" "${MDI_DIR}")

qt_add_resources(${APP_NAME} MDI
	BASE "${MDI_DIR}"
	PREFIX "mdi"
	FILES
	"${MDI_DIR}/account-circle.svg"
	"${MDI_DIR}/bell-badge-outline.svg"
	"${MDI_DIR}/bell-outline.svg"
	"${MDI_DIR}/chevron-left.svg"
	"${MDI_DIR}/compass.svg"
	"${MDI_DIR}/format-list-numbered.svg"
	"${MDI_DIR}/home.svg"
	"${MDI_DIR}/magnify.svg"
	"${MDI_DIR}/microphone-variant.svg"
	"${MDI_DIR}/pause.svg"
	"${MDI_DIR}/play.svg"
	"${MDI_DIR}/plus.svg"
	"${MDI_DIR}/repeat-off.svg"
	"${MDI_DIR}/repeat-once.svg"
	"${MDI_DIR}/repeat.svg"
	"${MDI_DIR}/shuffle-disabled.svg"
	"${MDI_DIR}/shuffle.svg"
	"${MDI_DIR}/skip-next.svg"
	"${MDI_DIR}/skip-previous.svg"
	"${MDI_DIR}/volume-high.svg"
	"${MDI_DIR}/volume-low.svg"
	"${MDI_DIR}/volume-medium.svg"
	"${MDI_DIR}/volume-mute.svg"
)
