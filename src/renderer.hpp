#pragma once

#include <SDL3/SDL.h>

bool MppRendererInit(SDL_Window* window);
void MppRendererQuit(SDL_Window* window);
void MppRendererSubmit(SDL_Window* window);