add_library(stb STATIC
        src/stb.cpp
)

target_include_directories(stb
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)