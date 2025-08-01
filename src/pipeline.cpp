#include <SDL3/SDL.h>

#include <cstddef>
#include <cstdint>

#include "pipeline.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "transform.hpp"

SDL_GPUGraphicsPipeline* MppCreateObjModelPipeline(SDL_GPUDevice* device, SDL_Window* window)
{
    SDL_GPUShader* fragShader = MppLoadShader(device, "obj_model.frag");
    SDL_GPUShader* vertShader = MppLoadShader(device, "obj_model.vert");
    if (!fragShader || !vertShader)
    {
        SDL_Log("Failed to load shader(s)");
        return nullptr;
    }
    SDL_GPUColorTargetDescription targets[1]{};
    SDL_GPUVertexBufferDescription buffers[2]{};
    SDL_GPUVertexAttribute attribs[3]{};
    targets[0].format = SDL_GetGPUSwapchainTextureFormat(device, window);
    buffers[0].slot = 0;
    buffers[0].pitch = sizeof(uint32_t);
    buffers[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    buffers[0].instance_step_rate = 0;
    buffers[1].slot = 1;
    buffers[1].pitch = sizeof(MppTransform);
    buffers[1].input_rate = SDL_GPU_VERTEXINPUTRATE_INSTANCE;
    buffers[1].instance_step_rate = 0;
    attribs[0].location = 0;
    attribs[0].format = SDL_GPU_VERTEXELEMENTFORMAT_UINT;
    attribs[0].offset = 0;
    attribs[0].buffer_slot = 0;
    attribs[1].location = 1;
    attribs[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
    attribs[1].offset = offsetof(MppTransform, Position);
    attribs[1].buffer_slot = 1;
    attribs[2].location = 2;
    attribs[2].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT;
    attribs[2].offset = offsetof(MppTransform, Rotation);
    attribs[2].buffer_slot = 1;
    SDL_GPUGraphicsPipelineCreateInfo info{};
    info.vertex_shader = vertShader;
    info.fragment_shader = fragShader;
    info.target_info.color_target_descriptions = targets;
    info.target_info.num_color_targets = 1;
    info.target_info.depth_stencil_format = MppGetDepthTextureFormat(device);
    info.target_info.has_depth_stencil_target = true;
    info.vertex_input_state.vertex_buffer_descriptions = buffers;
    info.vertex_input_state.num_vertex_buffers = 2;
    info.vertex_input_state.vertex_attributes = attribs;
    info.vertex_input_state.num_vertex_attributes = 3;
    info.depth_stencil_state.enable_depth_test = true;
    info.depth_stencil_state.enable_depth_write = true;
    info.depth_stencil_state.compare_op = SDL_GPU_COMPAREOP_LESS;
    info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_BACK;
    info.rasterizer_state.front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE;
    SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(device, &info);
    if (!pipeline)
    {
        SDL_Log("Failed to create graphics pipeline: %s", SDL_GetError());
        return nullptr;
    }
    SDL_ReleaseGPUShader(device, fragShader);
    SDL_ReleaseGPUShader(device, vertShader);
    return pipeline;
}