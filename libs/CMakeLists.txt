set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "libs")

# plog
set(INCLUDE_DIRS ${INCLUDE_DIRS} "${PLOG_DIR}/include")

# glfw
# Overriding some glfw default options.
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
add_subdirectory(glfw)
set(INCLUDE_DIRS ${INCLUDE_DIRS} "${GLFW_DIR}/include")
set(LIBRARIES ${LIBRARIES} glfw)

# glm
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${GLM_DIR})
#Because glm project cmake is imperfect for our project, so we don't use its cmake system.
set(LIBRARY_NAME "glm")
file(GLOB HEADERS "glm/glm/*.hpp" "glm/glm/*.h")
file(GLOB SOURCES "glm/glm/*.cpp")
if(SOURCES)
    add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
    set(LIBRARIES ${LIBRARIES} ${LIBRARY_NAME})    
    set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})
endif(SOURCES)

# imgui
if(USE_IMGUI_BIND)
    set(INCLUDE_DIRS ${INCLUDE_DIRS} ${IMGUI_DIR}) #to include imgui headers.
    set(LIBRARY_NAME "imgui")
    file(GLOB HEADERS "${IMGUI_DIR}/imgui/*.hpp" "${IMGUI_DIR}/imgui/*.h")
    file(GLOB SOURCES "${IMGUI_DIR}/imgui/*.cpp")
    if(SOURCES)
        add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
        set(LIBRARIES ${LIBRARIES} ${LIBRARY_NAME})
        set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})
    endif(SOURCES)
    
endif(USE_IMGUI_BIND)

# stb
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${STB_DIR})
set(LIBRARY_NAME "stb")
file(GLOB HEADERS "stb/stb/*.hpp" "stb/stb/*.h")
file(GLOB SOURCES "stb/stb/*.cpp")
if(SOURCES)
    add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
    set(LIBRARIES ${LIBRARIES} ${LIBRARY_NAME}) 
    set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})
endif(SOURCES)

#gli
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${GLI_DIR})
#Because gli project cmake is imperfect for our project, so we don't use its cmake system.
set(LIBRARY_NAME "gli")
file(GLOB HEADERS "gli/gli/*.hpp" "gli/gli/*.h")
file(GLOB SOURCES "gli/gli/*.cpp")
if(SOURCES)
    add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
    set(LIBRARIES ${LIBRARIES} ${LIBRARY_NAME}) 
    set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})
endif(SOURCES)

set(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
set(LIBRARIES ${LIBRARIES} PARENT_SCOPE)
