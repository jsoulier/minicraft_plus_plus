#include <SDL3/SDL.h>

#include "window.hpp"

static constexpr const char* kTitle = "Minicraft++";
static constexpr int kWidth = 960;
static constexpr int kHeight = 720;

bool MppWindow::Init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }
    Window = SDL_CreateWindow(kTitle, kWidth, kHeight, SDL_WINDOW_RESIZABLE);
    if (!Window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    return true;
}

void MppWindow::Quit()
{
    SDL_DestroyWindow(Window);
    SDL_Quit();
    Window = nullptr;
}

SDL_Window* MppWindow::GetHandle()
{
    return Window;
}