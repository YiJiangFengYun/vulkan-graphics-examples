set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "libs")

# Overriding some glfw default options.
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
# glfw generation
add_subdirectory(glfw)

set(LIBRARIES ${LIBRARIES} glfw)

# imgui generation
set(LIBRARY_NAME "imgui")
file(GLOB HEADERS "${IMGUI_DIR}/imgui/*.hpp" "${IMGUI_DIR}/imgui/*.h")
file(GLOB SOURCES "${IMGUI_DIR}/imgui/*.cpp")
add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
set(LIBRARIES ${LIBRARIES} ${LIBRARY_NAME})
set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})


set(LIBRARIES ${LIBRARIES} PARENT_SCOPE)