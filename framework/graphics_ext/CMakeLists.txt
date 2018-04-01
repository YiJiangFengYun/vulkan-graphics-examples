set(LIBRARY_NAME "graphics_ext")
file(GLOB_RECURSE HEADERS *.hpp *.inl)
file(GLOB_RECURSE SOURCES *.cpp)
include_directories(${INCLUDE_DIRS})
add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})
set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${FOLDER_NAME})

# install
install (TARGETS ${LIBRARY_NAME} DESTINATION bin)
install (FILES ${HEADERS} DESTINATION include)
