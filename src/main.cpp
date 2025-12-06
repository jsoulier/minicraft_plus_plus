#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_AppResult SDLCALL SDL_AppInit(void** appstate, int argc, char** argv)
{
    return SDL_APP_SUCCESS;
}

SDL_AppResult SDLCALL SDL_AppIterate(void* appstate)
{
    return SDL_APP_SUCCESS;
}

SDL_AppResult SDLCALL SDL_AppEvent(void* appstate, SDL_Event* event)
{
    return SDL_APP_SUCCESS;
}

void SDLCALL SDL_AppQuit(void* appstate, SDL_AppResult result)
{

}