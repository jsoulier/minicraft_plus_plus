#include <SDL3/SDL.h>

#include "debug_group.hpp"

#ifndef NDEBUG
MppDebugGroup::MppDebugGroup(SDL_GPUCommandBuffer* commandBuffer, const char* name) : CommandBuffer{commandBuffer}
{
    SDL_PushGPUDebugGroup(CommandBuffer, name);
}

MppDebugGroup::~MppDebugGroup()
{
    SDL_PopGPUDebugGroup(CommandBuffer);
}
#endif