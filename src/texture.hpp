#pragma once

#include <SDL3/SDL.h>

#include <string_view>

SDL_GPUTexture* MppLoadTexture(SDL_GPUDevice* device, SDL_GPUCopyPass* copyPass, const std::string_view& path);
SDL_GPUTexture* MppCreateColorTexture(SDL_GPUDevice* device, SDL_Window* window, int width, int height, SDL_GPUTextureUsageFlags usage = 0);
SDL_GPUTextureFormat MppGetDepthTextureFormat(SDL_GPUDevice* device);
SDL_GPUTexture* MppCreateDepthTexture(SDL_GPUDevice* device, int width, int height, SDL_GPUTextureUsageFlags usage = 0);