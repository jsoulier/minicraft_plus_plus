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

set(GLM_BUILD_LIBRARY OFF)
add_subdirectory(lib/SDL)
add_subdirectory(lib/glm)
add_subdirectory(lib/json)
add_subdirectory(lib/tinyobjloader)
add_executable(minicraft++ WIN32
    lib/imgui/imgui.cpp
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_impl_sdl3.cpp
    lib/imgui/imgui_impl_sdlgpu3.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_widgets.cpp
    lib/stb/stb.c
    src/camera.cpp
    src/debug_group.cpp
    src/entity.cpp
    src/main.cpp
    src/mob_entity.cpp
    src/model.cpp
    src/obj_model.cpp
    src/pipeline.cpp
    src/player_entity.cpp
    src/renderer.cpp
    src/shader.cpp
    src/state.cpp
    src/texture.cpp
    src/ticks.cpp
    src/vox_model.cpp
    src/window.cpp
)
set_target_properties(minicraft++ PROPERTIES CXX_STANDARD 23)
target_include_directories(minicraft++ PRIVATE lib/imgui)
target_include_directories(minicraft++ PRIVATE lib/stb)
target_link_libraries(minicraft++ PRIVATE SDL3::SDL3)
target_link_libraries(minicraft++ PRIVATE glm)
target_link_libraries(minicraft++ PRIVATE nlohmann_json)
target_link_libraries(minicraft++ PRIVATE tinyobjloader)

find_package(Vulkan)
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
    package(${SPV})
    if(WIN32)
        package(${DXIL})
    elseif(APPLE)
        package(${MSL})
    endif()
    package(${JSON})
endfunction()
add_shader(obj_model.frag)
add_shader(obj_model.vert)
add_shader(sampler.comp)

function(add_model NAME)
    configure_file(res/models/${NAME}.obj ${BINARY_DIR} COPYONLY)
    configure_file(res/models/${NAME}.png ${BINARY_DIR} COPYONLY)
    configure_file(res/models/${NAME}.vox ${BINARY_DIR} COPYONLY)
endfunction()
add_model(default)

configure_file(LICENSE.txt ${BINARY_DIR} COPYONLY)
configure_file(README.md ${BINARY_DIR} COPYONLY)