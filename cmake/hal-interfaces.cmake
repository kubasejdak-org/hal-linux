include(FetchContent)
FetchContent_Declare(hal-interfaces
    GIT_REPOSITORY  https://gitlab.com/embeddedlinux/libs/hal-interfaces.git
    GIT_TAG         origin/master
)

FetchContent_GetProperties(hal-interfaces)
if (NOT hal-interfaces_POPULATED)
    FetchContent_Populate(hal-interfaces)
endif ()
