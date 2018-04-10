configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in"
	"${PROJECT_CONFIG_HEADER_DIR}/vgim/config.hpp"
)
# add the binary tree directory to the search path for include files
# include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(INCLUDE_DIRS ${INCLUDE_DIRS} ${IMGUI_DIR})

set(LIBRARY_NAME "vgim")
file(GLOB_RECURSE HEADERS *.hpp *.inl)
file(GLOB_RECURSE SOURCES *.cpp)
set(HEADERS ${HEADERS} "${PROJECT_CONFIG_HEADER_DIR}/vgim/config.hpp")
include_directories(${INCLUDE_DIRS})
add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})
set(LIBRARIES ${LIBRARIES} ${LIBRARY_NAME}) 

# install
install (TARGETS ${LIBRARY_NAME} DESTINATION bin)
install (FILES ${HEADERS} DESTINATION include)

set(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
set(LIBRARIES ${LIBRARIES} PARENT_SCOPE)


