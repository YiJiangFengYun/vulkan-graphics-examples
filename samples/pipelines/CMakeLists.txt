# add the binary tree directory to the search path for include files
# include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
set(EXE_NAME "pipelines")
file(GLOB_RECURSE HEADERS *.hpp *.inl)
file(GLOB_RECURSE SOURCES *.cpp)

if(SOURCES)
    add_executable(${EXE_NAME} ${HEADERS} ${SOURCES})
    target_link_libraries(${EXE_NAME} ${LIBRARIES})
    set_property(TARGET ${EXE_NAME} PROPERTY FOLDER ${FOLDER_NAME})
        
    # Compile shaders
    set(COMMON_SHADER_NAMES ${COMMON_SHADER_NAMES} "phong")
    set(COMMON_SHADER_NAMES ${COMMON_SHADER_NAMES} "toon")
    set(COMMON_SHADER_NAMES ${COMMON_SHADER_NAMES} "wireframe")
    set(SHADERS_DIR "${SHADERS_DIR}/${EXE_NAME}")    
    if(COMMON_SHADER_NAMES)
        foreach(SHADER_NAME ${COMMON_SHADER_NAMES})
            file(GLOB 
            SHADERS 
            "${SHADERS_DIR}/${SHADER_NAME}.vert" 
            "${SHADERS_DIR}/${SHADER_NAME}.frag" 
            "${SHADERS_DIR}/${SHADER_NAME}.geom" 
            "${SHADERS_DIR}/${SHADER_NAME}.tesc" 
            "${SHADERS_DIR}/${SHADER_NAME}.tese")
        
            foreach(SHADER ${SHADERS})
               set(SHADER_SPV "${SHADER}.spv")
               if(WIN32)
                   add_custom_command(TARGET ${EXE_NAME} PRE_BUILD
                       COMMAND "${Vulkan_BIN_DIR}/glslangValidator.exe" "-V" "${SHADER}" "-o" "${SHADER_SPV}"
                   )
               endif(WIN32)
            endforeach(SHADER ${SHADERS})
        endforeach(SHADER_NAME ${COMMON_SHADER_NAMES})
    endif(COMMON_SHADER_NAMES)
    
    # copy resource files
    set(RESOURCES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    add_custom_command(TARGET ${EXE_NAME} PRE_BUILD
                      COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${RESOURCES_DIR} ${RESOURCES_OUTPUT_DIR})
    
    # install
    install (TARGETS ${EXE_NAME} DESTINATION bin)
    install (FILES ${HEADERS} DESTINATION include)
endif(SOURCES)