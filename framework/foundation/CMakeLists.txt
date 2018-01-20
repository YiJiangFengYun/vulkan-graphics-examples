
# add the binary tree directory to the search path for include files
# include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(LIBRARY_NAME "foundation")
file(GLOB HEADERS *.hpp)
file(GLOB SOURCES *.cpp)
add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})

# install
install (TARGETS ${LIBRARY_NAME} DESTINATION bin)
install (FILES ${HEADERS} DESTINATION include)
