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
    src/minicraft++/audio.cpp
    src/minicraft++/color.cpp
    src/minicraft++/console.cpp
    src/minicraft++/entity/controller/controller.cpp
    src/minicraft++/entity/controller/creature.cpp
    src/minicraft++/entity/controller/melee.cpp
    src/minicraft++/entity/controller/npc.cpp
    src/minicraft++/entity/controller/player.cpp
    src/minicraft++/entity/entity.cpp
    src/minicraft++/entity/furniture/anvil.cpp
    src/minicraft++/entity/furniture/chest.cpp
    src/minicraft++/entity/furniture/furnace.cpp
    src/minicraft++/entity/furniture/furniture.cpp
    src/minicraft++/entity/furniture/lantern.cpp
    src/minicraft++/entity/furniture/recipe.cpp
    src/minicraft++/entity/furniture/workbench.cpp
    src/minicraft++/entity/item.cpp
    src/minicraft++/entity/mob/creature.cpp
    src/minicraft++/entity/mob/humanoid.cpp
    src/minicraft++/entity/mob/mob.cpp
    src/minicraft++/entity/mob/player.cpp
    src/minicraft++/entity/mob/sheep.cpp
    src/minicraft++/entity/mob/slime.cpp
    src/minicraft++/entity/mob/spider.cpp
    src/minicraft++/entity/mob/zombie.cpp
    src/minicraft++/entity/particle/hit.cpp
    src/minicraft++/entity/particle/particle.cpp
    src/minicraft++/entity/projectile/arrow.cpp
    src/minicraft++/entity/projectile/projectile.cpp
    src/minicraft++/gen.cpp
    src/minicraft++/input.cpp
    src/minicraft++/inventory.cpp
    src/minicraft++/item.cpp
    src/minicraft++/main.cpp
    src/minicraft++/menu.cpp
    src/minicraft++/renderer.cpp
    src/minicraft++/sprite.cpp
    src/minicraft++/tile.cpp
    src/minicraft++/util.cpp
    src/minicraft++/world.cpp
    src/minicraft++/level.cpp
)
set_target_properties(minicraft++ PROPERTIES CXX_STANDARD 20)
target_include_directories(minicraft++ PRIVATE src)
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
