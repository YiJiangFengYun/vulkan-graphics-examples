
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

    # Compile common shaders
    # set(COMMON_SHADER_NAMES ${COMMON_SHADER_NAMES} "ui")
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
                   add_custom_command(TARGET "${LIBRARY_NAME}" PRE_BUILD
                       COMMAND "${Vulkan_BIN_DIR}/glslangValidator.exe" "-V" "${SHADER}" "-o" "${SHADER_SPV}"
                   )
               endif(WIN32)
            endforeach(SHADER ${SHADERS})
        endforeach(SHADER_NAME ${COMMON_SHADER_NAMES})
    endif(COMMON_SHADER_NAMES)
endif()
