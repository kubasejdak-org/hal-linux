add_library(hal-uart EXCLUDE_FROM_ALL
    LinuxUart.cpp
)
add_library(hal::uart ALIAS hal-uart)

target_include_directories(hal-uart
    PUBLIC include
)

target_link_libraries(hal-uart
    PUBLIC hal::interfaces osal::cpp
    PRIVATE hal::linux-logger
)
