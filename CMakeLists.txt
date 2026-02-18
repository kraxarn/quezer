cmake_minimum_required(VERSION 3.16)

project(quezer
	LANGUAGES CXX
	VERSION 0.0.1
)

set(APP_NAME "${PROJECT_NAME}_app")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
	Core
	Multimedia
	Quick
	QuickControls2
	WebView
)
qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(${APP_NAME})
add_subdirectory(src)

include(qml.cmake)

target_include_directories(${APP_NAME} PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_definitions(${APP_NAME} PRIVATE
	# App info
	APP_NAME="${PROJECT_NAME}"
	APP_VERSION="v${PROJECT_VERSION}"
	# Qt config
	QT_NO_CAST_FROM_ASCII
	QT_NO_CAST_TO_ASCII
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(${APP_NAME} PRIVATE QT_QML_DEBUG)
endif ()

set_target_properties(${APP_NAME} PROPERTIES
	# macOS
	MACOSX_BUNDLE TRUE
	MACOSX_BUNDLE_GUI_IDENTIFIER "com.kraxarn.${PROJECT_NAME}"
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	# Windows
	WIN32_EXECUTABLE TRUE
)

target_link_libraries(${APP_NAME} PRIVATE
	Qt6::Core
	Qt6::Multimedia
	Qt6::Quick
	Qt6::QuickControls2
	Qt6::WebView
)

include(GNUInstallDirs)
install(TARGETS ${APP_NAME}
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
