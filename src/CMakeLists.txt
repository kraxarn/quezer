add_subdirectory(deezer)

target_sources(${APP_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
