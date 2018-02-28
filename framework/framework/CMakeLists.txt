set(VGF_VERSION_MAJOR "0")
set(VGF_VERSION_MINOR "0")
set(VGF_VERSION_PATCH "1")
set(VGF_VERSION_EXTRA "")
set(VGF_VERSION "${VGF_VERSION_MAJOR}.${VGF_VERSION_MINOR}")
set(VGF_VERSION_FULL "${VGF_VERSION}.${VGF_VERSION_PATCH}${VGF_VERSION_EXTRA}")

# add the binary tree directory to the search path for include files
# include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in"
	"${PROJECT_CONFIG_HEADER_DIR}/framework/config.hpp"
)

include_directories("${PROJECT_FRAMEWORK_DIR}")

include_directories("${GLFW_DIR}/include")
include_directories("${IMGUI_DIR}")

set(LIBRARY_NAME "framework")
file(GLOB_RECURSE HEADERS *.hpp *.inl)
file(GLOB_RECURSE SOURCES *.cpp)
set(HEADERS ${HEADERS} "${PROJECT_CONFIG_HEADER_DIR}/framework/config.hpp")
add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})

# install
install (TARGETS ${LIBRARY_NAME} DESTINATION bin)
install (FILES ${HEADERS} DESTINATION include)
