if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
else()
    set(DOXY_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in)
    set(DOXY_FILE ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

    configure_file(${DOXY_FILE_IN} ${DOXY_FILE} @ONLY)

    add_custom_target(doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
    
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION install/doc)
endif()