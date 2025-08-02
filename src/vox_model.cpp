#include <SDL3/SDL.h>

#include <string_view>

#include "vox_model.hpp"

bool MppVoxModel::Load(SDL_GPUDevice* device, SDL_GPUCopyPass* copyPass, const std::string_view& name)
{
    return true;
}

void MppVoxModel::Destroy(SDL_GPUDevice* device)
{
}