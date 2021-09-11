cmake_minimum_required(VERSION 3.19)

# Define application settings.
include(settings.cmake)

include(cmake/conan.cmake)
include(cmake/doxygen.cmake)
include(cmake/hal-interfaces.cmake)
include(cmake/osal.cmake)
include(cmake/platform.cmake)
include(cmake/utils.cmake)

project(hal-linux VERSION 1.0.0 LANGUAGES ASM C CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

conan_init()

# Project-wide compilation options.
add_compile_options(-Wall -Wextra -Wpedantic -Werror $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>)

add_subdirectory(${platform_SOURCE_DIR}/app ${platform_BINARY_DIR})
add_subdirectory(${hal-interfaces_SOURCE_DIR}/lib ${hal-interfaces_BINARY_DIR})
add_subdirectory(${osal_SOURCE_DIR}/lib ${osal_BINARY_DIR})
add_subdirectory(${utils_SOURCE_DIR}/lib ${utils_BINARY_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib)

# Generate documentation.
doxygen_addTarget(${CMAKE_CURRENT_SOURCE_DIR}/lib README.md)
