project(FastNoiseLite)
add_library(FastNoiseLite INTERFACE)
add_library(FastNoiseLite::FastNoiseLite ALIAS FastNoiseLite)
target_include_directories(FastNoiseLite INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})