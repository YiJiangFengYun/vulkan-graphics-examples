
# add the binary tree directory to the search path for include files
# include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
set(EXE_NAME "test-gemo")
file(GLOB_RECURSE HEADERS *.hpp)
file(GLOB_RECURSE SOURCES *.cpp)
add_executable(${EXE_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${EXE_NAME} ${LIBRARIES})

# install
install (TARGETS ${EXE_NAME} DESTINATION bin)
install (FILES ${HEADERS} DESTINATION include)

