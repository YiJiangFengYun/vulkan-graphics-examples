set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "libs")

# Overriding some glfw default options.
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
# glfw generation
add_subdirectory(glfw)

# glm generation
add_subdirectory(glm)

# imgui generation
set(LIBRARY_NAME "imgui")
file(GLOB HEADERS "${IMGUI_DIR}/imgui/*.hpp" "${IMGUI_DIR}/imgui/*.h")
file(GLOB SOURCES "${IMGUI_DIR}/imgui/*.cpp")
add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})

# stb generetion
set(LIBRARY_NAME "stb")
file(GLOB HEADERS "stb/stb/*.hpp" "stb/stb/*.h")
file(GLOB SOURCES "stb/stb/*.cpp")
if(SOURCES)
    add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
    set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})
endif(SOURCES)

#gli generation
#Because gli project cmake is imperfect, so we don't use its cmake system.
set(LIBRARY_NAME "gli")
file(GLOB HEADERS "gli/gli/*.hpp" "gli/gli/*.h")
file(GLOB SOURCES "gli/gli/*.cpp")
if(SOURCES)
    add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
    set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})
endif(SOURCES)
