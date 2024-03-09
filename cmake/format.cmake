# From: https://github.com/SFML/SFML/blob/master/cmake/Format.cmake

# Check executable exists
set(CLANG_FORMAT_EXECUTABLE "clang-format")
set(CLANG_FORMAT_FILES ${${PROJECT_NAME}_SOURCE})

if(NOT EXISTS ${CLANG_FORMAT_EXECUTABLE})
    find_program(CLANG_FORMAT_EXEC_TEMP ${CLANG_FORMAT_EXECUTABLE})
    if(CLANG_FORMAT_EXEC_TEMP)
        set(CLANG_FORMAT_EXECUTABLE ${CLANG_FORMAT_EXEC_TEMP})
        unset(CLANG_FORMAT_EXEC_TEMP)
    else()
        message(FATAL_ERROR "Unable to find clang-format executable: \"${CLANG_FORMAT_EXECUTABLE}\"")
    endif()
endif()

# Run
execute_process(COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${CLANG_FORMAT_FILES})
