cmake_minimum_required(VERSION 3.23)

project(pokay LANGUAGES C)

add_library(pokaylib STATIC)

target_sources(pokaylib
    PRIVATE
        src/pokaylib.c
        src/maps/map-Pallet-town.c
    PUBLIC
        include/abilities.h
        include/config.h
        include/moves.h
        include/pokaylib.h
        include/pokemons.h
)

add_executable(pokay)

target_sources(pokay
    PRIVATE
        src/main.c
)

target_link_libraries(pokay
    PRIVATE
        pokaylib
)

target_include_directories(pokaylib
    PUBLIC
        include
)

add_subdirectory(3rd-party/raylib)

target_link_libraries(pokaylib
    PUBLIC
        raylib
)

# Copy resources to build directory

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_dependencies(pokay copy_assets)

