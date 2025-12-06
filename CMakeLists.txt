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

add_subdirectory(lib/FastNoiseLite)
add_subdirectory(lib/SDL)
add_subdirectory(lib/box2d)
add_subdirectory(lib/savepoint)

add_executable(minicraft++ WIN32
    src/main.cpp
)
target_link_libraries(minicraft++ PRIVATE
    FastNoiseLite::FastNoiseLite
    SDL3::SDL3
    box2d::box2d
    savepoint::savepoint
)