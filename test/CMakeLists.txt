set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "test")

include_directories("${GLM_DIR}")
include_directories("${PLOG_DIR}/include")
include_directories("${PROJECT_FRAMEWORK_DIR}")

include_directories("${GLFW_DIR}/include")
include_directories("${IMGUI_DIR}")

include_directories("${PROJECT_TEST_DIR}")

set(LIBRARIES ${LIBRARIES} foundation)
set(LIBRARIES ${LIBRARIES} graphics)
set(LIBRARIES ${LIBRARIES} framework)

set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(SHADERS_DIR "${RESOURCES_DIR}/shaders")
set(RESOURCES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(SHADERS_OUTPUT_DIR "${RESOURCES_OUTPUT_DIR}/shaders")

add_subdirectory(testlib)

add_subdirectory(test-gemo)
add_subdirectory(test-triangle-2d)
add_subdirectory(test-triangle)
