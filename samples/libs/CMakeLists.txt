set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(FOLDER_NAME "samples_libs")

# assimp generation
SET(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "If the supplementary tools for Assimp are built in addition to the library.")
SET(ASSIMP_BUILD_TESTS OFF CACHE BOOL "If the test suite for Assimp is built in addition to the library.")
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
add_subdirectory(assimp)
set(INCLUDE_DIRS ${INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/assimp/include")
set(INCLUDE_DIRS ${INCLUDE_DIRS} "${CMAKE_CURRENT_BINARY_DIR}/assimp/include") #include assimp config file
set(LIBRARIES ${LIBRARIES} assimp)

set(LIBRARIES ${LIBRARIES} PARENT_SCOPE)
set(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)