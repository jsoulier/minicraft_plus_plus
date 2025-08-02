#pragma once

#include <SDL3/SDL.h>

class MppWindow
{
public:
    bool Init();
    void Quit();
    SDL_Window* GetHandle();

private:
    SDL_Window* Window;
};