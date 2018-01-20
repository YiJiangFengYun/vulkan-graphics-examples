include_directories("${GLM_DIR}/glm")
include_directories("${PLOG_DIR}/include")
include_directories("${PROJECT_FRAMEWORK_DIR}/foundation")
include_directories("${PROJECT_FRAMEWORK_DIR}/graphics")
include_directories("${GLFW_DIR}/include")
include_directories("${IMGUI_DIR}")


set(LIBRARIES ${LIBRARIES} foundation)
set(LIBRARIES ${LIBRARIES} graphics)
set(LIBRARIES ${LIBRARIES} framework)

# add_subdirectory(sampleslib)

# set(LIBRARIES ${LIBRARIES} sampleslib)

set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(RESOURCES_OUTPUT_DIR "${RUNTIME_OUTPUT_DIRECTORY}")
set(SHADERS_DIR "${RESOURCES_DIR}/shaders")
set(SHADERS_OUTPUT_DIR "${RESOURCES_OUTPUT_DIR}/shaders")

# add_subdirectory(test-gemo)
# add_subdirectory(test-triangle-2d)
# add_subdirectory(test-triangle)
