#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <cstdint>

#include <minicraft++/audio.hpp>
#include <minicraft++/input.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/version.hpp>
#include <minicraft++/world.hpp>

static uint64_t frameTicks;
static uint64_t savedTicks;

static uint64_t GetTicks()
{
    return frameTicks + savedTicks;
}

SDL_AppResult SDLCALL SDL_AppInit(void** appstate, int argc, char** argv)
{
    SDL_SetAppMetadata("minicraft++", kMppVersion.GetString().data(), nullptr);
#ifndef NDEBUG
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
#endif
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        MppLog("Failed to initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!MppRendererInit())
    {
        MppLog("Failed to initialize renderer");
        return SDL_APP_FAILURE;
    }
    if (!MppAudioInit())
    {
        MppLog("Failed to initialize audio");
        // Intentionally not a fatal error
    }
    if (!MppWorldInit())
    {
        MppLog("Failed to initialize world");
        return SDL_APP_FAILURE;
    }
    savedTicks = MppWorldGetTicks();
    return SDL_APP_CONTINUE;
}

void SDLCALL SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    uint64_t ticks = GetTicks();
    MppInputQuit();
    MppWorldSave(ticks, true);
    MppWorldQuit();
    MppAudioQuit();
    MppRendererQuit();
    SDL_Quit();
}

SDL_AppResult SDLCALL SDL_AppIterate(void* appstate)
{
    frameTicks++;
    uint64_t ticks = GetTicks();
    MppInputUpdate(ticks);
    MppWorldUpdate(ticks);
    MppWorldSave(ticks, false);
    MppWorldRender();
    MppInputRender();
    MppRendererSubmit();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDLCALL SDL_AppEvent(void* appstate, SDL_Event* event)
{
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_TEXT_INPUT:
        MppInputHandle(event);
        break;
    }
    return SDL_APP_CONTINUE;
}