set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "framework")

# glm dependency
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${GLM_DIR})
set(LIBRARIES ${LIBRARIES} glm)
# plog dependency
set(INCLUDE_DIRS ${INCLUDE_DIRS} "${PLOG_DIR}/include")
# stb dependency
set(INCLUDE_DIRS ${INCLUDE_DIRS} "${STB_DIR}")
# gli dependency
set(INCLUDE_DIRS ${INCLUDE_DIRS} "${GLI_DIR}")

# imgui dependency
if(USE_IMGUI_BIND)
    set(INCLUDE_DIRS ${INCLUDE_DIRS} ${IMGUI_DIR}) #to include imgui headers.
    set(LIBRARIES ${LIBRARIES} imgui)
endif(USE_IMGUI_BIND)
# framework include.
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_FRAMEWORK_DIR})

add_subdirectory(foundation)
set(LIBRARIES ${LIBRARIES} foundation)
add_subdirectory(graphics)
set(LIBRARIES ${LIBRARIES} graphics)

if(USE_IMGUI_BIND)
    add_subdirectory(vgim)
    set(LIBRARIES ${LIBRARIES} vgim)            
endif(USE_IMGUI_BIND)

add_subdirectory(framework)
set(LIBRARIES ${LIBRARIES} framework)

set(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
set(LIBRARIES ${LIBRARIES} PARENT_SCOPE)