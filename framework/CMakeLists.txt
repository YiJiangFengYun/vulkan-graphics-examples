set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "framework")

include_directories("${GLM_DIR}")
include_directories("${PLOG_DIR}/include")

add_subdirectory(foundation)
add_subdirectory(graphics)

if(USE_IMGUI_BIND)
    add_subdirectory(vgim)    
endif(USE_IMGUI_BIND)

add_subdirectory(framework)