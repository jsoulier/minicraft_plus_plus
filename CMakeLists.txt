cmake_minimum_required(VERSION 3.24)
project(minicraft++)

set(BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BINARY_DIR})
make_directory(${BINARY_DIR})

configure_file(LICENSE.txt ${BINARY_DIR} COPYONLY)
configure_file(README.md ${BINARY_DIR} COPYONLY)
configure_file(res/textures/spritesheet.png ${BINARY_DIR} COPYONLY)

add_subdirectory(lib/FastNoiseLite)
add_subdirectory(lib/SDL)
add_subdirectory(lib/box2d)
add_subdirectory(lib/savepoint)

add_executable(minicraft++ WIN32
    src/ai_controller.cpp
    src/color.cpp
    src/controller.cpp
    src/entity.cpp
    src/humanoid_entity.cpp
    src/item.cpp
    src/item_entity.cpp
    src/level.cpp
    src/main.cpp
    src/mob_entity.cpp
    src/player_controller.cpp
    src/player_entity.cpp
    src/renderer.cpp
    src/slime_entity.cpp
    src/sprite.cpp
    src/surface_level.cpp
    src/tile.cpp
    src/underground_level.cpp
    src/world.cpp
)
set_target_properties(minicraft++ PROPERTIES CXX_STANDARD 20)
target_link_libraries(minicraft++ PRIVATE
    FastNoiseLite::FastNoiseLite
    SDL3::SDL3
    box2d::box2d
    savepoint::savepoint
)