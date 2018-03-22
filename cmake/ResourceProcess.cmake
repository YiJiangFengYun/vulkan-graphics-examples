macro(res_process target res_output_dir res_dir shader_dir shader_names )
    # message("res_processs, target:${target}")
    # message("res_processs, res_output_dir:${res_output_dir}")
    # message("res_processs, res_dir:${res_dir}")
    # message("res_processs, shader_dir:${shader_dir}")
    # message("res_processs, shader_names:${shader_names}")
    set(SHADER_NAMES ${shader_names})
    # Compile shaders
    if(SHADER_NAMES)
        foreach(SHADER_NAME ${SHADER_NAMES})
            file(GLOB 
            SHADERS 
            "${shader_dir}/${SHADER_NAME}.vert" 
            "${shader_dir}/${SHADER_NAME}.frag" 
            "${shader_dir}/${SHADER_NAME}.geom" 
            "${shader_dir}/${SHADER_NAME}.tesc" 
            "${shader_dir}/${SHADER_NAME}.tese")                                 
            foreach(SHADER ${SHADERS})
               set(SHADER_SPV "${SHADER}.spv")
               if(WIN32)
                   message("res_processs, excute add_custom_command")
                   add_custom_command(TARGET ${target} PRE_BUILD
                       COMMAND "${Vulkan_BIN_DIR}/glslangValidator.exe" "-V" "${SHADER}" "-o" "${SHADER_SPV}"
                   )
               endif(WIN32)
            endforeach(SHADER ${SHADERS})
        endforeach(SHADER_NAME ${SHADER_NAMES})
    endif(SHADER_NAMES)
    
    set(RESOURCES_OUTPUT_DIR ${res_output_dir})
    if (RESOURCES_OUTPUT_DIR)
        # copy resource files
        add_custom_command(TARGET ${target} PRE_BUILD
                          COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${res_dir} ${RESOURCES_OUTPUT_DIR})
    endif(RESOURCES_OUTPUT_DIR)
endmacro(res_process)


