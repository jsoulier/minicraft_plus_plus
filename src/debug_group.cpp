#include <SDL3/SDL.h>

#include "debug_group.hpp"

MppDebugGroup::MppDebugGroup(SDL_GPUCommandBuffer* commandBuffer, const char* name)
#ifndef NDEBUG
    : commandBuffer{commandBuffer}
#endif
{
#ifndef NDEBUG
    SDL_PushGPUDebugGroup(commandBuffer, name);
#endif
}

MppDebugGroup::~MppDebugGroup()
{
#ifndef NDEBUG
    SDL_PopGPUDebugGroup(commandBuffer);
#endif
}