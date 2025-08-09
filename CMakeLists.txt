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

set(SDLIMAGE_AVIF OFF)
set(SDLIMAGE_BMP OFF)
set(SDLIMAGE_GIF OFF)
set(SDLIMAGE_JPG OFF)
set(SDLIMAGE_JXL OFF)
set(SDLIMAGE_LBM OFF)
set(SDLIMAGE_PCX OFF)
set(SDLIMAGE_PNG ON)
set(SDLIMAGE_PNM OFF)
set(SDLIMAGE_QOI OFF)
set(SDLIMAGE_SVG OFF)
set(SDLIMAGE_TGA OFF)
set(SDLIMAGE_TIF OFF)
set(SDLIMAGE_WEBP OFF)
set(SDLIMAGE_XCF OFF)
set(SDLIMAGE_XPM OFF)
set(SDLIMAGE_XV OFF)

add_subdirectory(lib/SDL)
add_subdirectory(lib/SDL_image)
add_subdirectory(lib/SDL_ttf)
add_executable(minicraft++ WIN32
    lib/sqlite3/sqlite3.c
    src/main.cpp
    src/window.cpp
)
set_target_properties(minicraft++ PROPERTIES CXX_STANDARD 23)
target_include_directories(minicraft++ PRIVATE lib/json)
target_include_directories(minicraft++ PRIVATE lib/sqlite3)
target_link_libraries(minicraft++ PRIVATE SDL3::SDL3)
target_link_libraries(minicraft++ PRIVATE SDL3_image::SDL3_image)
target_link_libraries(minicraft++ PRIVATE SDL3_ttf::SDL3_ttf)

if(MSVC)
    find_package(Vulkan)
endif()
function(add_shader FILE)
    set(DEPENDS ${ARGN})
    set(GLSL ${CMAKE_SOURCE_DIR}/res/shaders/${FILE})
    set(SPV ${CMAKE_SOURCE_DIR}/res/shaders/bin/${FILE}.spv)
    set(DXIL ${CMAKE_SOURCE_DIR}/res/shaders/bin/${FILE}.dxil)
    set(MSL ${CMAKE_SOURCE_DIR}/res/shaders/bin/${FILE}.msl)
    set(JSON ${CMAKE_SOURCE_DIR}/res/shaders/bin/${FILE}.json)
    function(compile PROGRAM SOURCE OUTPUT)
        add_custom_command(
            OUTPUT ${OUTPUT}
            COMMAND ${PROGRAM} ${SOURCE} -o ${OUTPUT} -I src
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            DEPENDS ${SOURCE} ${DEPENDS}
            COMMENT ${OUTPUT}
        )
        get_filename_component(NAME ${OUTPUT} NAME)
        string(REPLACE . _ NAME ${NAME})
        set(NAME compile_${NAME})
        add_custom_target(${NAME} DEPENDS ${OUTPUT})
        add_dependencies(minicraft++ ${NAME})
    endfunction()
    if (MSVC AND ${Vulkan_glslc_FOUND})
        set(SHADERCROSS lib/SDL_shadercross/msvc/shadercross.exe)
        compile(${Vulkan_GLSLC_EXECUTABLE} ${GLSL} ${SPV} -g)
        compile(${SHADERCROSS} ${SPV} ${DXIL})
        compile(${SHADERCROSS} ${SPV} ${MSL})
        compile(${SHADERCROSS} ${SPV} ${JSON})
    endif()
    function(package OUTPUT)
        get_filename_component(NAME ${OUTPUT} NAME)
        set(BINARY ${BINARY_DIR}/${NAME})
        add_custom_command(
            OUTPUT ${BINARY}
            COMMAND ${CMAKE_COMMAND} -E copy ${OUTPUT} ${BINARY}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            DEPENDS ${OUTPUT}
            COMMENT ${BINARY}
        )
        string(REPLACE . _ NAME ${NAME})
        set(NAME package_${NAME})
        add_custom_target(${NAME} DEPENDS ${BINARY})
        add_dependencies(minicraft++ ${NAME})
    endfunction()
    if(WIN32)
        package(${DXIL})
    elseif(APPLE)
        package(${MSL})
    elseif(UNIX)
        package(${SPV})
    endif()
    package(${JSON})
endfunction()

configure_file(LICENSE.txt ${BINARY_DIR} COPYONLY)
configure_file(README.md ${BINARY_DIR} COPYONLY)
configure_file(res/textures/spritesheet.png ${BINARY_DIR} COPYONLY)
configure_file(res/fonts/raster_forge.ttf ${BINARY_DIR} COPYONLY)