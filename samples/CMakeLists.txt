set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "samples")

include_directories("${GLM_DIR}")
include_directories("${PLOG_DIR}/include")
include_directories("${PROJECT_FRAMEWORK_DIR}")
include_directories("${GLFW_DIR}/include")
include_directories("${IMGUI_DIR}")

set(LIBRARIES ${LIBRARIES} foundation)
set(LIBRARIES ${LIBRARIES} graphics)
if (USE_IMGUI_BIND)
    set(LIBRARIES ${LIBRARIES} vgim)    
endif()
set(LIBRARIES ${LIBRARIES} framework)

add_subdirectory(libs)
include_directories("libs/assimp/include")

set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(SHADERS_DIR "${RESOURCES_DIR}/shaders")
set(RESOURCES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(SHADERS_OUTPUT_DIR "${RESOURCES_OUTPUT_DIR}/shaders")

include_directories("${PROJECT_SAMPLES_DIR}")
add_subdirectory(sampleslib)

add_subdirectory(chalet)
add_subdirectory(triangle)
add_subdirectory(pipelines)