set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "framework")

set(INCLUDE_DIRS ${INCLUDE_DIRS} ${GLM_DIR})
set(LIBRARIES ${LIBRARIES} glm)
set(INCLUDE_DIRS ${INCLUDE_DIRS} "${PLOG_DIR}/include")
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${IMGUI_DIR}) #to include imgui headers.
set(LIBRARIES ${LIBRARIES} imgui)
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