#pragma once

#include <SDL3/SDL.h>

#include "model.hpp"

class MppTransform;
class MppTime;

bool MppRendererInit(SDL_Window* window);
void MppRendererQuit(SDL_Window* window);
void MppRendererBeginFrame(SDL_Window* window);
void MppRendererUpdate(const MppTransform& target, const MppTime& time);
void MppRendererDraw(MppModel model, const MppTransform& transform);
void MppRendererEndFrame();