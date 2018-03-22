
# add the binary tree directory to the search path for include files
# include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(LIBRARY_NAME "testlib")
file(GLOB_RECURSE HEADERS *.hpp *.h *.inl)
file(GLOB_RECURSE SOURCES *.cpp)

if(SOURCES)
    include_directories(${INCLUDE_DIRS})
    add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
    set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})
    set(LIBRARIES ${LIBRARIES} ${LIBRARY_NAME})    
    set(LIBRARIES ${LIBRARIES} PARENT_SCOPE)
    
    # install
    install (TARGETS ${LIBRARY_NAME} DESTINATION bin)
    install (FILES ${HEADERS} DESTINATION include)

    # set(COMMON_SHADER_NAMES ${COMMON_SHADER_NAMES} "ui")
    set(RESOURCES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    if(COMMON_SHADER_NAMES)
        res_process("${LIBRARY_NAME}" "${RESOURCES_OUTPUT_DIR}" "${RESOURCES_DIR}" "${SHADERS_DIR}" "${COMMON_SHADER_NAMES}")
    endif(COMMON_SHADER_NAMES)
endif()
