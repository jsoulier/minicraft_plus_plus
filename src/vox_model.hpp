#pragma once

#include <SDL3/SDL.h>

#include <string_view>

class MppVoxModel
{
public:
    /* TODO: implement */
    bool Load(SDL_GPUDevice* device, SDL_GPUCopyPass* copyPass, const std::string_view& name);
    void Destroy(SDL_GPUDevice* device);
};