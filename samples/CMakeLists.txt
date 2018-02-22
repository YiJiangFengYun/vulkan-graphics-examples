set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "samples")

include_directories("${GLM_DIR}")
include_directories("${PLOG_DIR}/include")
include_directories("${PROJECT_FRAMEWORK_DIR}")
include_directories("${GLFW_DIR}/include")
include_directories("${IMGUI_DIR}")

include_directories("${PROJECT_SAMPLES_DIR}")


set(LIBRARIES ${LIBRARIES} foundation)
set(LIBRARIES ${LIBRARIES} graphics)
if (USE_IMGUI_BIND)
    set(LIBRARIES ${LIBRARIES} vgim)    
endif()
set(LIBRARIES ${LIBRARIES} framework)


set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(SHADERS_DIR "${RESOURCES_DIR}/shaders")
set(RESOURCES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(SHADERS_OUTPUT_DIR "${RESOURCES_OUTPUT_DIR}/shaders")

add_subdirectory(sampleslib)

add_subdirectory(chalet)
add_subdirectory(triangle)