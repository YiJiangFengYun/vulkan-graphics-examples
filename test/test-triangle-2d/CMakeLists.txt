# add the binary tree directory to the search path for include files
# include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
set(EXE_NAME "test-triangle-2d")
file(GLOB_RECURSE HEADERS *.hpp *.inl)
file(GLOB_RECURSE SOURCES *.cpp)
add_executable(${EXE_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${EXE_NAME} ${LIBRARIES})


# copy resource files
set(RESOURCES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(SHADERS_OUTPUT_DIR "${RESOURCES_OUTPUT_DIR}/shaders")
set(SHADER_NAME "triangle-2d")
file(GLOB 
     SHADERS 
     "${SHADERS_DIR}/${SHADER_NAME}.vert" 
     "${SHADERS_DIR}/${SHADER_NAME}.frag" 
     "${SHADERS_DIR}/${SHADER_NAME}.geom" 
     "${SHADERS_DIR}/${SHADER_NAME}.tesc" 
     "${SHADERS_DIR}/${SHADER_NAME}.tese")

foreach(SHADER ${SHADERS})
    set(SHADER_SPV "${SHADER}.spv")
    string(REPLACE "${SHADERS_DIR}" "${SHADERS_OUTPUT_DIR}" SHADER_OUT "${SHADER_SPV}")
    if(WIN32)
        add_custom_command(TARGET "${EXE_NAME}" PRE_BUILD
            COMMAND "${Vulkan_BIN_DIR}/glslangValidator.exe" "-V" "${SHADER}" "-o" "${SHADER_SPV}"
        )
    endif(WIN32)
 endforeach()
 add_custom_command(TARGET ${EXE_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${RESOURCES_DIR} ${RESOURCES_OUTPUT_DIR})

# install
install (TARGETS ${EXE_NAME} DESTINATION bin)
install (FILES ${HEADERS} DESTINATION include)

