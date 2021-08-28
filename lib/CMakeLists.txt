cmake_minimum_required(VERSION 3.19)

# Project-wide compilation options.
add_compile_options(-Wall -Wextra -Wpedantic -Werror $<$<COMPILE_LANGUAGE:CXX>:-std=c++17> $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>)

add_subdirectory(gpio)
add_subdirectory(i2c)
add_subdirectory(spi)
add_subdirectory(uart)
add_subdirectory(utils)
