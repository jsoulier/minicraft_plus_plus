#include <SDL3/SDL.h>

#include "debug_group.hpp"

#ifndef NDEBUG
MppDebugGroup::MppDebugGroup(SDL_GPUCommandBuffer* commandBuffer, const char* name)
    : commandBuffer{commandBuffer}
{
    SDL_PushGPUDebugGroup(commandBuffer, name);
}

MppDebugGroup::~MppDebugGroup()
{
    SDL_PopGPUDebugGroup(commandBuffer);
}
#endif