set(VG_VERSION_MAJOR "0")
set(VG_VERSION_MINOR "0")
set(VG_VERSION_PATCH "1")
set(VG_VERSION_EXTRA "")
set(VG_VERSION "${VG_VERSION_MAJOR}.${VG_VERSION_MINOR}")
set(VG_VERSION_FULL "${VG_VERSION}.${VG_VERSION_PATCH}${VG_VERSION_EXTRA}")

option(VG_ENABLE_VALIDATION_LAYERS "Enable features of vulkan validation layers." ON)
option(VG_ENABLE_COST_TIMER "Enable cost timer for the important step." OFF)

set(LIBRARY_NAME "graphics")

#Pre depth default shader.
compile_shader("${CMAKE_CURRENT_SOURCE_DIR}/shader" "pre_depth_default")
add_custom_command (
  OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/pre_depth_vert_default_code.c"
  COMMAND "embed_file_to_code" "VG_PRE_DEPTH_VERT_DEFAULT_CODE" "${CMAKE_CURRENT_SOURCE_DIR}/shader/pre_depth_default.vert.spv" "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/pre_depth_vert_default_code.c"
  MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/shader/pre_depth_default.vert.spv"
  DEPENDS "embed_file_to_code"
)
add_custom_command (
  OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/pre_depth_frag_default_code.c"
  COMMAND "embed_file_to_code" "VG_PRE_DEPTH_FRAG_DEFAULT_CODE" "${CMAKE_CURRENT_SOURCE_DIR}/shader/pre_depth_default.frag.spv" "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/pre_depth_frag_default_code.c"
  MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/shader/pre_depth_default.frag.spv"
  DEPENDS "embed_file_to_code"
)

#Light depth default shader.
compile_shader("${CMAKE_CURRENT_SOURCE_DIR}/shader" "light_depth_default")
add_custom_command (
  OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/light_depth_vert_default_code.c"
  COMMAND "embed_file_to_code" "VG_LIGHT_DEPTH_VERT_DEFAULT_CODE" "${CMAKE_CURRENT_SOURCE_DIR}/shader/light_depth_default.vert.spv" "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/light_depth_vert_default_code.c"
  MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/shader/light_depth_default.vert.spv"
  DEPENDS "embed_file_to_code"
)
add_custom_command (
  OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/light_depth_frag_default_code.c"
  COMMAND "embed_file_to_code" "VG_LIGHT_DEPTH_FRAG_DEFAULT_CODE" "${CMAKE_CURRENT_SOURCE_DIR}/shader/light_depth_default.frag.spv" "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/light_depth_frag_default_code.c"
  MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/shader/light_depth_default.frag.spv"
  DEPENDS "embed_file_to_code"
)

#Light point distance default shader.
compile_shader("${CMAKE_CURRENT_SOURCE_DIR}/shader" "light_point_dist_default")
add_custom_command (
  OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/light_point_dist_vert_default_code.c"
  COMMAND "embed_file_to_code" "VG_LIGHT_POINT_DIST_VERT_DEFAULT_CODE" "${CMAKE_CURRENT_SOURCE_DIR}/shader/light_point_dist_default.vert.spv" "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/light_point_dist_vert_default_code.c"
  MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/shader/light_point_dist_default.vert.spv"
  DEPENDS "embed_file_to_code"
)
add_custom_command (
  OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/light_point_dist_frag_default_code.c"
  COMMAND "embed_file_to_code" "VG_LIGHT_POINT_DIST_FRAG_DEFAULT_CODE" "${CMAKE_CURRENT_SOURCE_DIR}/shader/light_point_dist_default.frag.spv" "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/light_point_dist_frag_default_code.c"
  MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/shader/light_point_dist_default.frag.spv"
  DEPENDS "embed_file_to_code"
)


configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config/config.hpp.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/config.hpp"
)
set(INCLUDE_DIRS ${INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE HEADERS "${LIBRARY_NAME}/*.hpp" "${LIBRARY_NAME}/*.inl")
file(GLOB_RECURSE SOURCES "${LIBRARY_NAME}/*.cpp" "${LIBRARY_NAME}/*.c")
set(SOURCES ${SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/pre_depth_vert_default_code.c")
set(SOURCES ${SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/pre_depth_frag_default_code.c")
set(SOURCES ${SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/light_depth_vert_default_code.c")
set(SOURCES ${SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/light_depth_frag_default_code.c")
set(SOURCES ${SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/light_point_dist_vert_default_code.c")
set(SOURCES ${SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/pass/light_point_dist_frag_default_code.c")
include_directories(${INCLUDE_DIRS})
add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})
set(LIBRARIES ${LIBRARIES} ${LIBRARY_NAME})

# install
install (TARGETS ${LIBRARY_NAME} DESTINATION bin)
install (FILES ${HEADERS} DESTINATION include)

set(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
set(LIBRARIES ${LIBRARIES} PARENT_SCOPE)
