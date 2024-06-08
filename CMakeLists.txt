cmake_minimum_required(VERSION 3.23)

project(pokaylib LANGUAGES C)

add_library(pokaylib STATIC)

add_subdirectory(3rd-party/raylib)

target_link_libraries(pokaylib
    PUBLIC
        raylib
)

# only for pokaylib; raylib has too much warnings...
add_compile_options(
    -std=c2x
    # warnings
    -Wall
    -Wextra
    -Wpedantic
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

target_sources(pokaylib
    PRIVATE # .c files
        src/pokaylib.c
        src/maps/map-Pallet-town.c
    PUBLIC # .h files
        include/abilities.h
        include/config.h
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

# Copy resources to build directory
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_dependencies(pokexec copy_assets)
