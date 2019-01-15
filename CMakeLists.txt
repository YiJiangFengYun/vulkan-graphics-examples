cmake_minimum_required(VERSION 3.8.2)

project(vulkan-graphics-samples)

#include cmake codes
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ResourceProcess)

#Set project variables.
#    Global
set(PROJECT_NAME "vulkan-graphics-samples")

set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "1")
set(VERSION_EXTRA "")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")
set(VERSION_FULL "${VERSION}.${VERSION_PATCH}${VERSION_EXTRA}")
set(LIB_SUFFIX "" CACHE STRING "Takes an empty string or 64. Directory where lib will be installed: lib or lib64")

#find neccessay packages
find_package(Vulkan REQUIRED)
find_package(Boost REQUIRED)
find_package(Doxygen)

# options
# option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(USE_IMGUI_BIND "Build imgui bind" ON)
option(BUILD_SAMPLES "Build the sample programs" ON)
option(BUILD_TESTS "Build the test programs" ON)
option(BUILD_DOCUMENTATION "Create and install the html based api documentation (requires Doxygen)" ${DOXYGEN_FOUND})
if (MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" ON)
endif()
option(USE_D2D_WSI "Build the project using Direct to Display swapchain" OFF)
option(USE_WAYLAND_WSI "Build the project using Wayland swapchain" OFF)

#path variables
set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(PROJECT_FRAMEWORK_DIR ${PROJECT_DIR}/framework)
set(PROJECT_BIN_DIR ${PROJECT_DIR}/doc)
set(PROJECT_BUILD_DIR ${PROJECT_DIR}/build)
set(PROJECT_CMAKE_DIR ${PROJECT_DIR}/cmake)
set(PROJECT_DOC_DIR ${PROJECT_DIR}/doc)
set(PROJECT_LIB_DIR ${PROJECT_DIR}/lib)
set(PROJECT_LIBS_DIR ${PROJECT_DIR}/libs)
set(PROJECT_SAMPLES_DIR ${PROJECT_DIR}/samples)
set(PROJECT_TEST_DIR ${PROJECT_DIR}/test)


# Third party library directory
set(GLFW_DIR ${PROJECT_LIBS_DIR}/glfw)
set(GLM_DIR ${PROJECT_LIBS_DIR}/glm)
set(IMGUI_DIR ${PROJECT_LIBS_DIR}/imgui)
set(LIBUV_DIR ${PROJECT_LIBS_DIR}/libuv)
set(PLOG_DIR ${PROJECT_LIBS_DIR}/plog)
set(STB_DIR ${PROJECT_LIBS_DIR}/stb)
set(GLI_DIR ${PROJECT_LIBS_DIR}/gli)

set(LIBRARIES) #init variable
add_subdirectory(libs)

# Set compiler specific flags
# if (MSVC)
#     if (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
#         foreach (flag CMAKE_C_FLAGS
#                       CMAKE_C_FLAGS_DEBUG
#                       CMAKE_C_FLAGS_RELEASE
#                       CMAKE_C_FLAGS_MINSIZEREL
#                       CMAKE_C_FLAGS_RELWITHDEBINFO)

#             if (${flag} MATCHES "/MD")
#                 string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
#             endif()
#             if (${flag} MATCHES "/MDd")
#                 string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
#             endif()

#         endforeach()
#     endif()
# endif()

# set (CMAKE_CXX_STANDARD 11)

# vulkan

if(WIN32)
    # find_library(ASSIMP_LIBRARIES NAMES assimp libassimp.dll.a PATHS ${CMAKE_SOURCE_DIR}/libs/assimp)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR")
else(WIN32)
    # find_package(ASSIMP REQUIRED)
    find_package(Threads REQUIRED)
    if(USE_D2D_WSI)
        message("Using direct to display extension...")
        add_definitions(-D_DIRECT2DISPLAY)
    elseif(USE_WAYLAND_WSI)
        find_package(Wayland REQUIRED)
        if (NOT WAYLAND_FOUND)
            message(FATAL_ERROR "Wayland development package not found")
        endif ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WAYLAND_KHR")
        set(INCLUDE_DIRS ${INCLUDE_DIRS} ${WAYLAND_INCLUDE_DIR})
    elseif(USE_D2D_WSI)
        find_package(XCB REQUIRED)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_XCB_KHR")
    endif(USE_D2D_WSI)
    # Todo : android?
endif(WIN32)

set(INCLUDE_DIRS ${INCLUDE_DIRS} ${Vulkan_INCLUDE_DIRS})
set(LIBRARIES ${LIBRARIES} ${Vulkan_LIBRARIES})
message(STATUS "Vulkan_INCLUDE_DIR: ${Vulkan_INCLUDE_DIR}")
message(STATUS "Vulkan_INCLUDE_DIRS: ${Vulkan_INCLUDE_DIRS}")
message(STATUS "Vulkan_LIBRARY: ${Vulkan_LIBRARY}")
message(STATUS "Vulkan_LIBRARIES: ${Vulkan_LIBRARIES}")

#find boost
#now we only use boost head file code.
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "Boost_VERSION: ${Boost_VERSION}")

# generators
add_subdirectory(generator)

# global third party library include and link directories
# set(INCLUDE_DIRS ${INCLUDE_DIRS} "${LIBUV_DIR}/include")

# framework generation
add_subdirectory(framework)

# enable test
enable_testing ()

# samples generation
if(BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

# tests generation
if(BUILD_TESTS)
    add_subdirectory(test)
endif()

# doucments generation
if (BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif(BUILD_DOCUMENTATION)

# install
# install (FILES "${PROJECT_BINARY_DIR}/VGFConfig.hpp"
#   DESTINATION include)




