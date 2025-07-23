#pragma once

#include <SDL3/SDL.h>

#ifndef NDEBUG
#define MPP_DEBUG_GROUP(commandBuffer) MppDebugGroup debugGroup(commandBuffer, __func__)
#else
#define MPP_DEBUG_GROUP(commandBuffer)
#endif

#ifndef NDEBUG
struct MppDebugGroup
{
    MppDebugGroup(SDL_GPUCommandBuffer* commandBuffer, const char* name);
    ~MppDebugGroup();

    SDL_GPUCommandBuffer* commandBuffer;
};
#endif