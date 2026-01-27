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
add_subdirectory(lib/savepoint)

add_executable(minicraft++ WIN32
    src/audio.cpp
    src/color.cpp
    src/console.cpp
    src/entity.cpp
    src/entity/anvil.cpp
    src/entity/chest.cpp
    src/entity/controller.cpp
    src/entity/creature.cpp
    src/entity/furnace.cpp
    src/entity/furniture.cpp
    src/entity/hostile_controller.cpp
    src/entity/humanoid.cpp
    src/entity/item.cpp
    src/entity/mob.cpp
    src/entity/player.cpp
    src/entity/recipe.cpp
    src/entity/sheep.cpp
    src/entity/slime.cpp
    src/entity/workbench.cpp
    src/entity/zombie.cpp
    src/gen.cpp
    src/input.cpp
    src/inventory.cpp
    src/item.cpp
    src/main.cpp
    src/menu.cpp
    src/renderer.cpp
    src/save.cpp
    src/sprite.cpp
    src/tile.cpp
    src/util.cpp
    src/world.cpp
)
set_target_properties(minicraft++ PROPERTIES CXX_STANDARD 20)
target_link_libraries(minicraft++ PRIVATE
    FastNoiseLite::FastNoiseLite
    SDL3::SDL3
    savepoint::savepoint
)
target_precompile_headers(minicraft++ PRIVATE
    <algorithm>
    <array>
    <cctype>
    <cmath>
    <cstddef>
    <cstdint>
    <exception>
    <filesystem>
    <format>
    <limits>
    <memory>
    <sstream>
    <string>
    <string_view>
    <unordered_map>
    <vector>
)
