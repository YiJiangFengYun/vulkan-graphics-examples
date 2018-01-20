
# add the binary tree directory to the search path for include files
# include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(EXE_NAME "test-triangle-2d")
file(GLOB HEADERS *.hpp)
file(GLOB SOURCES *.cpp)
add_executable(${EXE_NAME} STATIC ${HEADERS} ${SOURCES})
target_link_libraries(${EXE_NAME} ${LIBRARIES})

# copy resource files
file(GLOB 
     SHADERS 
     "${EXE_NAME}.vert" 
     "${EXE_NAME}.frag" 
     "${EXE_NAME}.geom" 
     "${EXE_NAME}.tesc" 
     "${EXE_NAME}.tese")

foreach(SHADER ${SHADERS})
    add_custom_command(
        WORKING_DIR ${Vulkan_BIN_DIR}
        COMMAND glslangValidator
        ARGS "-V" "${SHADERS_DIR}/${SHADER}" "-O" "${SHADERS_DIR}/${SHADER}.spv"
    )
    configure_file("${SHADERS_DIR}/${SHADER}.spv" "${SHADERS_OUTPUT_DIR}/${SHADER}.spv" COPYONLY)
 endforeach()

 configure_file("${RESOURCES_DIR}/textures/texture.jpg" "${RESOURCES_OUTPUT_DIR}/textures/texture.jpg" COPYONLY)

# install
install (TARGETS ${EXE_NAME} DESTINATION bin)
install (FILES ${HEADERS} DESTINATION include)
