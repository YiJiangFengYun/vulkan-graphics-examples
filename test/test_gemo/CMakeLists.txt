
# add the binary tree directory to the search path for include files
# include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
set(EXE_NAME "test_gemo")
file(GLOB_RECURSE HEADERS *.hpp *.inl)
file(GLOB_RECURSE SOURCES *.cpp)

include_directories(${INCLUDE_DIRS})
add_executable(${EXE_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${EXE_NAME} ${LIBRARIES})
set_property(TARGET ${EXE_NAME} PROPERTY FOLDER ${FOLDER_NAME})

# install
install (TARGETS ${EXE_NAME} DESTINATION bin)
install (FILES ${HEADERS} DESTINATION include)

# test
add_test (${EXE_NAME} ${EXE_NAME})

