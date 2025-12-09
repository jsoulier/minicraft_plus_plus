#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "app.hpp"
#include "renderer.hpp"
#include "version.hpp"
#include "world.hpp"

static MppApp app;

bool MppApp::Init()
{
    SDL_SetAppMetadata("minicraft++", kVersion.GetString().data(), nullptr);
#ifndef NDEBUG
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
#endif
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }
    if (!Renderer.Init())
    {
        SDL_Log("Failed to initialize renderer");
        return false;
    }
    return true;
}

void MppApp::Quit()
{
    Renderer.Quit();
    SDL_Quit();
}

bool MppApp::Update()
{
    Renderer.Draw(MppSprite::Debug(kMppColorRed), 16, 32, MppRenderer::LayerDebug);
    Renderer.Draw(MppSprite::Character('A', kMppColorGreen), 32, 32, MppRenderer::LayerDebug);
    Renderer.Draw(MppSprite::Character('P', kMppColorBlue), 48, 32, MppRenderer::LayerDebug);
    Renderer.Update();
    return true;
}

bool MppApp::Handle(SDL_Event* event)
{
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return false;
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        break;
    }
    return true;
}

SDL_AppResult SDLCALL SDL_AppInit(void** appstate, int argc, char** argv)
{
    return app.Init() ? SDL_APP_CONTINUE : SDL_APP_FAILURE;
}

SDL_AppResult SDLCALL SDL_AppIterate(void* appstate)
{
    return app.Update() ? SDL_APP_CONTINUE : SDL_APP_SUCCESS;
}

SDL_AppResult SDLCALL SDL_AppEvent(void* appstate, SDL_Event* event)
{
    return app.Handle(event) ? SDL_APP_CONTINUE : SDL_APP_SUCCESS;
}

void SDLCALL SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    app.Quit();
}