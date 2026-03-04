add_subdirectory(deezer)
add_subdirectory(items)
add_subdirectory(models)
add_subdirectory(player)
add_subdirectory(qml)
add_subdirectory(util)

target_sources(${APP_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
