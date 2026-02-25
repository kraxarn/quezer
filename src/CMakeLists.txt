add_subdirectory(deezer)
add_subdirectory(qml)

target_sources(${APP_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
