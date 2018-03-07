set(VG_VERSION_MAJOR "0")
set(VG_VERSION_MINOR "0")
set(VG_VERSION_PATCH "1")
set(VG_VERSION_EXTRA "")
set(VG_VERSION "${VG_VERSION_MAJOR}.${VG_VERSION_MINOR}")
set(VG_VERSION_FULL "${VG_VERSION}.${VG_VERSION_PATCH}${VG_VERSION_EXTRA}")

option(VG_LOG_COST_TIME "Logging cost time info for the important step." ON)

# add the binary tree directory to the search path for include files
# include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in"
	"${PROJECT_CONFIG_HEADER_DIR}/graphics/config.hpp"
)

set(LIBRARY_NAME "graphics")
file(GLOB_RECURSE HEADERS *.hpp *.inl)
file(GLOB_RECURSE SOURCES *.cpp)
set(HEADERS ${HEADERS} "${PROJECT_CONFIG_HEADER_DIR}/framework/config.hpp")
include_directories(${INCLUDE_DIRS})
add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})

# install
install (TARGETS ${LIBRARY_NAME} DESTINATION bin)
install (FILES ${HEADERS} DESTINATION include)
