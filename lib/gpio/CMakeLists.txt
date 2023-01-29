add_library(hal-gpio EXCLUDE_FROM_ALL
    LinuxGpio.cpp
)
add_library(hal::gpio ALIAS hal-gpio)

target_include_directories(hal-gpio
    PUBLIC include
)

find_library(gpiod NAMES gpiod)

target_link_libraries(hal-gpio
    PUBLIC gpiod hal::interfaces
)
