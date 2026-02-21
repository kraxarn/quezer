add_subdirectory(api)
add_subdirectory(objects)

target_sources(${APP_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/apiresponse.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/cypher.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/deezerclient.cpp
)
