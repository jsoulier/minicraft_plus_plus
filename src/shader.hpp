#pragma once

#include <SDL3/SDL.h>

#include <string_view>

SDL_GPUShader* MppLoadShader(SDL_GPUDevice* device, const std::string_view& name);
SDL_GPUComputePipeline* MppLoadComputePipeline(SDL_GPUDevice* device, const std::string_view& name);