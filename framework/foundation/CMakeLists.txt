configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in"
	"${PROJECT_CONFIG_HEADER_DIR}/foundation/config.hpp"
)

set(LIBRARY_NAME "foundation")
file(GLOB_RECURSE HEADERS *.hpp *.inl)
file(GLOB_RECURSE SOURCES *.cpp)
set(HEADERS ${HEADERS} "${PROJECT_CONFIG_HEADER_DIR}/foundation/config.hpp")
include_directories(${INCLUDE_DIRS})
add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})

# install
install (TARGETS ${LIBRARY_NAME} DESTINATION bin)
install (FILES ${HEADERS} DESTINATION include)
