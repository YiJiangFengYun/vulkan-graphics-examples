set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "framework")

set(FD_PLOG_ID 101)
set(VG_PLOG_ID 201)
set(VG_VULKAN_PLOG_ID 202)
set(VG_COST_TIME_PLOG_ID 203)
set(VGF_PLOG_ID 301)
set(VGIM_PLOG_ID 401)

add_subdirectory(foundation)
add_subdirectory(graphics)
add_subdirectory(graphics_ext)

if(USE_IMGUI_BIND)
    add_subdirectory(vgim)           
endif(USE_IMGUI_BIND)

add_subdirectory(framework)

set(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
set(LIBRARIES ${LIBRARIES} PARENT_SCOPE)