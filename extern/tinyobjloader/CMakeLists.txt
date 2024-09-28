message(STATUS "Fetching tinyobjloader...")

FetchContent_Declare(tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG v1.0.6)
FetchContent_MakeAvailable(tinyobjloader)

message(STATUS "tinyobjloader Fetched")
