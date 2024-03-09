set(CLANG_TIDY_EXECUTABLE "clang-tidy")

if(NOT EXISTS ${CLANG_TIDY_EXECUTABLE})
    find_program(CLANG_TIDY_EXEC_TEMP ${CLANG_TIDY_EXECUTABLE})
    if(CLANG_TIDY_EXEC_TEMP)
        set(CLANG_TIDY_EXECUTABLE ${CLANG_TIDY_EXEC_TEMP})
        unset(CLANG_TIDY_EXEC_TEMP)
    else()
        message(FATAL_ERROR "Unable to find clang-tidy executable: \"${CLANG_TIDY_EXECUTABLE}\"")
    endif()
endif()

set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY_EXECUTABLE})
message(${CLANG_TIDY_EXECUTABLE})