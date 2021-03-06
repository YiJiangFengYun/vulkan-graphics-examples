# add the binary tree directory to the search path for include files
# include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
set(EXE_NAME "triangle_2d")
file(GLOB_RECURSE HEADERS *.hpp *.inl)
file(GLOB_RECURSE SOURCES *.cpp)

if(SOURCES)
    include_directories(${INCLUDE_DIRS})
    add_executable(${EXE_NAME} ${HEADERS} ${SOURCES})
    target_link_libraries(${EXE_NAME} ${LIBRARIES})
    set_property(TARGET ${EXE_NAME} PROPERTY FOLDER ${FOLDER_NAME})

    # Compile shaders
    set(COMMON_SHADER_NAMES ${COMMON_SHADER_NAMES} "triangle_2d")
    set(SHADERS_DIR "${SHADERS_DIR}/${EXE_NAME}")
    set(RESOURCES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    res_process("${EXE_NAME}" "${RESOURCES_OUTPUT_DIR}" "${RESOURCES_DIR}" "${SHADERS_DIR}" "${COMMON_SHADER_NAMES}")
    
    # install
    install (TARGETS ${EXE_NAME} DESTINATION bin)
    install (FILES ${HEADERS} DESTINATION include)

    # test
    add_test (${EXE_NAME} ${EXE_NAME})
endif()