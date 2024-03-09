cmake_minimum_required(VERSION 3.21)

project(tiny-shell LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED true)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCE CONFIGURE_DEPENDS "source/*.[ch]")

# Formatter and linter
include(cmake/format.cmake)

option(ENABLE_LINTER "Use clang-tidy as linter" off)
if (ENABLE_LINTER)
    include(cmake/tidy.cmake)
endif()

# Executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCE})
