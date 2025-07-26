#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "renderer.hpp"

static constexpr const char* kAppName = "Minicraft Plus Plus";
static constexpr const char* kWindowTitle = "Minicraft++";
static constexpr int kWindowWidth = 960;
static constexpr int kWindowHeight = 720;

static SDL_Window* window;

SDL_AppResult SDLCALL SDL_AppInit(void** appstate, int argc, char** argv)
{
    SDL_SetAppMetadata(kAppName, nullptr, nullptr);
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
    window = SDL_CreateWindow(kWindowTitle, kWindowWidth, kWindowHeight, SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!MppRendererInit(window))
    {
        SDL_Log("Failed to initialize renderer");
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

void SDLCALL SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    MppRendererQuit(window);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_AppResult SDLCALL SDL_AppIterate(void* appstate)
{
    MppRendererBeginFrame(window);
    MppRendererEndFrame(window);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDLCALL SDL_AppEvent(void* appstate, SDL_Event* event)
{
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}