set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "samples")

add_subdirectory(libs)

set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(SHADERS_DIR "${RESOURCES_DIR}/shaders")
set(RESOURCES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(SHADERS_OUTPUT_DIR "${RESOURCES_OUTPUT_DIR}/shaders")

set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SAMPLES_DIR})
add_subdirectory(sampleslib)

add_subdirectory(chalet)
add_subdirectory(triangle)
add_subdirectory(pipelines)
add_subdirectory(texture)
add_subdirectory(cubemaps)
add_subdirectory(texture_array)
add_subdirectory(mesh)
add_subdirectory(external_uniform_buffer)
add_subdirectory(push_constants)
add_subdirectory(specialization_constants)
add_subdirectory(off_screen)

# sampler include directories and libraries is used by itself
# set(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
# set(LIBRARIES ${LIBRARIES} PARENT_SCOPE)