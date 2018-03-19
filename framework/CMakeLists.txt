set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "framework")

set(FD_PLOG_ID 101)
set(VG_PLOG_ID 201)
set(VG_VULKAN_PLOG_ID 202)
set(VG_COST_TIME_PLOG_ID 203)
set(VGF_PLOG_ID 301)
set(VGIM_PLOG_ID 401)

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