cmake_minimum_required(VERSION 3.23)

project(pokaylib LANGUAGES C)

set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(pokaylib STATIC)
target_sources(pokaylib
    PRIVATE # .c files
        src/pokaylib.c
    PUBLIC # .h files
        include/abilities.h
        include/config.h
        include/maps.h
        include/moves.h
        include/pokaylib.h
        include/pokemons.h
)

add_executable(pokexec)
target_sources(pokexec
    PRIVATE
        src/main.c
)

target_link_libraries(pokexec
    PRIVATE
        pokaylib
)

target_include_directories(pokaylib
    PUBLIC
        include
)

target_compile_options(pokaylib
    PUBLIC
        # Warnings
        -Wall
        -Wextra
        -Wformat=2
        -Wno-unused-parameter
        -Wshadow
        -Wwrite-strings
        -Wstrict-prototypes
        -Wold-style-definition
        -Wredundant-decls
        -Wnested-externs
        -Wmissing-include-dirs
)
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(pokaylib
        PUBLIC
            -Werror
    )
endif()

add_subdirectory(3rd-party/raylib)
target_link_libraries(pokaylib
    PUBLIC
        raylib
)

# Copy resources to build directory
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_dependencies(pokexec copy_assets)
