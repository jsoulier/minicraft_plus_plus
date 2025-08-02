#pragma once

#include <SDL3/SDL.h>

class MppWindow;

SDL_GPUGraphicsPipeline* MppCreateObjModelPipeline(SDL_GPUDevice* device, MppWindow& window);