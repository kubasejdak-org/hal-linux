add_library(hal-linux-logger INTERFACE)
add_library(hal::linux-logger ALIAS hal-linux-logger)

target_include_directories(hal-linux-logger
    INTERFACE include
)

target_link_libraries(hal-linux-logger
    INTERFACE utils::logger
)
