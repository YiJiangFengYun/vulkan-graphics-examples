set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "test")

set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(SHADERS_DIR "${RESOURCES_DIR}/shaders")
set(RESOURCES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(SHADERS_OUTPUT_DIR "${RESOURCES_OUTPUT_DIR}/shaders")

set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_TEST_DIR})

add_subdirectory(test_gemo)

# sampler include directories and libraries is used by itself
# set(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
# set(LIBRARIES ${LIBRARIES} PARENT_SCOPE)