#pragma once

#include <SDL3/SDL.h>

#ifndef NDEBUG
#define MPP_DEBUG_GROUP(...) MppDebugGroup debugGroup(__VA_ARGS__, __func__)
#else
#define MPP_DEBUG_GROUP(...)
#endif

#ifndef NDEBUG
struct MppDebugGroup
{
    MppDebugGroup(SDL_GPUCommandBuffer* commandBuffer, const char* name);
    ~MppDebugGroup();

    SDL_GPUCommandBuffer* CommandBuffer;
};
#endif