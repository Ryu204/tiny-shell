cmake_minimum_required(VERSION 3.21)

project(tiny-shell LANGUAGES C VERSION 0.1)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED true)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/source/core/config.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/source/core/config.h")

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.[ch]")

# Formatter, linter, documentation
option(ENABLE_FORMAT "Use clang-format as formatter" off)
if (ENABLE_FORMAT)
    include(cmake/format.cmake)
endif()

option(ENABLE_LINTER "Use clang-tidy as linter" off)
if (ENABLE_LINTER)
    include(cmake/tidy.cmake)
endif()

option(ENABLE_DOCS "Generate documentation with doxygen" off)
if (ENABLE_DOCS)
    include(cmake/docs.cmake)
endif()

# Executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCE})
# Microsoft specifics
target_compile_definitions(${PROJECT_NAME} PRIVATE "_CRT_SECURE_NO_DEPRECATE")
