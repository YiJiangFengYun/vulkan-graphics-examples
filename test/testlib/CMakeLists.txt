
# add the binary tree directory to the search path for include files
# include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(LIBRARY_NAME "testlib")
file(GLOB_RECURSE HEADERS *.hpp *.h *.inl)
file(GLOB_RECURSE SOURCES *.cpp)

if(SOURCES)
    add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
    set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})
    set(LIBRARIES ${LIBRARIES} ${LIBRARY_NAME} PARENT_SCOPE)    

    # install
    install (TARGETS ${LIBRARY_NAME} DESTINATION bin)
    install (FILES ${HEADERS} DESTINATION include)
endif()
