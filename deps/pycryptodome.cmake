include(FetchContent)

FetchContent_Declare(pycryptodome
	GIT_REPOSITORY https://github.com/Legrandin/pycryptodome.git
	GIT_TAG v3.23.0
)

message(STATUS "Downloading pycryptodome")
FetchContent_MakeAvailable(pycryptodome)

add_library(pycryptodome)

target_sources(pycryptodome PRIVATE
	"${pycryptodome_SOURCE_DIR}/src/blowfish.c"
	"${pycryptodome_SOURCE_DIR}/src/raw_cbc.c"
)

target_include_directories(pycryptodome PUBLIC
	"${pycryptodome_SOURCE_DIR}/src"
)

target_compile_definitions(pycryptodome PRIVATE
	HAVE_STDINT_H
	HAVE_POSIX_MEMALIGN
)

enable_language(C)
target_link_libraries(${APP_NAME} PRIVATE pycryptodome)