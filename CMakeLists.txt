cmake_minimum_required(VERSION 3.22)

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

if (USE_CONAN)
    conan_init_wrapper()
    conan_init_profile()
    conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR} BUILD missing PROFILE_BUILD default PROFILE_HOST conan-profile)
    conan_load_buildinfo()
    conan_basic_setup()
endif ()

# Project-wide compilation options.
add_compile_options(-Wall -Wextra -Wpedantic -Werror $<$<COMPILE_LANGUAGE:CXX>:-std=c++20>)

add_subdirectory(${platform_SOURCE_DIR}/lib ${platform_BINARY_DIR})
add_subdirectory(${hal-interfaces_SOURCE_DIR}/lib ${hal-interfaces_BINARY_DIR})
add_subdirectory(${osal_SOURCE_DIR}/lib ${osal_BINARY_DIR})
add_subdirectory(${utils_SOURCE_DIR}/lib ${utils_BINARY_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib)

doxygen_addTarget(${CMAKE_CURRENT_SOURCE_DIR}/lib README.md)
