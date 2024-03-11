configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/config.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/config"
)

# set(DOXYGEN_EXECUTABLE "doxygen")
# set(DOXYGEN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/docs/config")

# if(NOT EXISTS ${DOXYGEN_EXECUTABLE})
#     find_program(DOXYGEN_EXEC_TEMP ${DOXYGEN_EXECUTABLE})
#     if(DOXYGEN_EXEC_TEMP)
#         set(DOXYGEN_EXECUTABLE ${DOXYGEN_EXEC_TEMP})
#         unset(DOXYGEN_EXEC_TEMP)
#     else()
#         message(FATAL_ERROR "Unable to find doxygen executable: \"${DOXYGEN_EXECUTABLE}\"")
#     endif()
# endif()

# # Run
# execute_process(COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_FILE})
