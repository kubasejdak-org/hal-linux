cmake_minimum_required(VERSION 3.21)

# Project-wide compilation options.
add_compile_options(-Wall -Wextra -Wpedantic -Werror $<$<COMPILE_LANGUAGE:CXX>:-std=c++20> $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>)

add_subdirectory(gpio)
add_subdirectory(i2c)
add_subdirectory(logger)
add_subdirectory(spi)
add_subdirectory(uart)

add_library(hal-linux INTERFACE)
add_library(hal::linux ALIAS hal-linux)

target_link_libraries(hal-linux
    INTERFACE hal::gpio hal::i2c hal::spi hal::uart
)
