add_library(hal-spi EXCLUDE_FROM_ALL
    LinuxSpi.cpp
)
add_library(hal::spi ALIAS hal-spi)

target_include_directories(hal-spi
    PUBLIC include
)

target_link_libraries(hal-spi
    PUBLIC hal::interfaces utils::types osal::cpp
)
