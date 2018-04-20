# add the binary tree directory to the search path for include files
# include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
set(EXE_NAME "passes")
file(GLOB_RECURSE HEADERS *.hpp *.inl)
file(GLOB_RECURSE SOURCES *.cpp)

if(SOURCES)
    include_directories(${INCLUDE_DIRS})
    add_executable(${EXE_NAME} ${HEADERS} ${SOURCES})
    target_link_libraries(${EXE_NAME} ${LIBRARIES})
    message("target link libraries ${EXE_NAME}: ${LIBRARIES}")
    set_property(TARGET ${EXE_NAME} PROPERTY FOLDER ${FOLDER_NAME})
        
    set(COMMON_SHADER_NAMES ${COMMON_SHADER_NAMES} "gbuffer")
    set(COMMON_SHADER_NAMES ${COMMON_SHADER_NAMES} "composition")
    set(COMMON_SHADER_NAMES ${COMMON_SHADER_NAMES} "to_trunk")
    set(COMMON_SHADER_NAMES ${COMMON_SHADER_NAMES} "transparent")
    set(COMMON_SHADER_NAMES ${COMMON_SHADER_NAMES} "only_color_test")
    set(SHADERS_DIR "${SHADERS_DIR}/${EXE_NAME}")
    set(RESOURCES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    res_process("${EXE_NAME}" "${RESOURCES_OUTPUT_DIR}" "${RESOURCES_DIR}" "${SHADERS_DIR}" "${COMMON_SHADER_NAMES}")

    # install
    install (TARGETS ${EXE_NAME} DESTINATION bin)
    install (FILES ${HEADERS} DESTINATION include)
endif(SOURCES)