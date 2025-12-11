#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <savepoint.hpp>

#include <filesystem>
#include <string_view>

#include "renderer.hpp"
#include "version.hpp"
#include "version.hpp"
#include "world.hpp"

static const std::filesystem::path kPrefPath = SDL_GetPrefPath(nullptr, "minicraft++");
static const std::filesystem::path kSavepoint = kPrefPath / "minicraft++.sqlite3";

static MppWorld world;
static MppRenderer renderer;
static Savepoint savepoint;

SDL_AppResult SDLCALL SDL_AppInit(void** appstate, int argc, char** argv)
{
    SDL_SetAppMetadata("minicraft++", kVersion.GetString().data(), nullptr);
#ifndef NDEBUG
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
#endif
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!renderer.Init())
    {
        SDL_Log("Failed to initialize renderer");
        return SDL_APP_FAILURE;
    }
    SavepointSetLogFunction([](const std::string_view& string)
    {
        SDL_Log("%s", string.data());
    });
    SavepointStatus status = savepoint.Open(kSavepoint.string().data(), kVersion);
    if (status == SavepointStatus::Failed)
    {
        SDL_Log("Failed to open savepoint: %s", kSavepoint.string().data());
        return SDL_APP_FAILURE;
    }
    if (!world.Init(savepoint, status))
    {
        SDL_Log("Failed to initialize world");
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDLCALL SDL_AppIterate(void* appstate)
{
    world.Update(renderer, 0.0f, 0.0f);
    renderer.Draw(MppSprite(kMppColorRed), 16, 32, MppRenderer::LayerDebug);
    renderer.Draw(MppSprite('A', kMppColorGreen), 32, 32, MppRenderer::LayerDebug);
    renderer.Draw(MppSprite('P', kMppColorBlue), 48, 32, MppRenderer::LayerDebug);
    renderer.Update(0.0f);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDLCALL SDL_AppEvent(void* appstate, SDL_Event* event)
{
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        break;
    }
    return SDL_APP_CONTINUE;
}

void SDLCALL SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    // TODO: save
    // savepoint.Save();
    world.Quit();
    savepoint.Close();
    renderer.Quit();
    SDL_Quit();
}