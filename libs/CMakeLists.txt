set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "libs")

# glfw generation
add_subdirectory(glfw)
set(LIBRARIES ${LIBRARIES} glfw)

# imgui generation
include_directories(${IMGUI_DIR})
# include_directories(${PROJECT_LIBS_DIR}) #to include imgui headers.

set(LIBRARY_NAME "imgui")
file(GLOB HEADERS ${IMGUI_DIR}/*.hpp ${IMGUI_DIR}/*.h)
file(GLOB SOURCES ${IMGUI_DIR}/*.cpp)
add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
set(LIBRARIES ${LIBRARIES} ${LIBRARY_NAME})
set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})


set(LIBRARIES ${LIBRARIES} PARENT_SCOPE)