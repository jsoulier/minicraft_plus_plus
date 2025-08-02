#include <SDL3/SDL.h>

#include "renderer.hpp"
#include "state.hpp"
#include "ticks.hpp"
#include "transform.hpp"
#include "window.hpp"

static constexpr const char* kAppName = "Minicraft Plus Plus";

static MppWindow window;
static MppRenderer renderer;
static MppTicks ticks;
static MppState state;

bool MppStateInit()
{
    SDL_SetAppMetadata(kAppName, nullptr, nullptr);
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
    if (!window.Init())
    {
        SDL_Log("Failed to initialize window");
        return false;
    }
    if (!renderer.Init(window))
    {
        SDL_Log("Failed to initialize renderer");
        return false;
    }
    ticks = MppTicks{0};
    state = MppState::Init;
    return true;
}

void MppStateQuit()
{
    renderer.Quit(window);
    window.Quit();
}

static MppState Init()
{
    return MppState::Play;
}

static MppState Play()
{
    renderer.Update({} /* TODO: player transform */, ticks);
    /* TODO: update world */
    renderer.Draw(MppModelDefault, MppTransform{glm::vec3{ 0.0f,   0.0f, -16.0f}, 0.0f});
    renderer.Draw(MppModelDefault, MppTransform{glm::vec3{ 32.0f,  0.0f, -16.0f}, 0.0f});
    renderer.Draw(MppModelDefault, MppTransform{glm::vec3{-32.0f,  0.0f, -16.0f}, 0.0f});
    renderer.Draw(MppModelDefault, MppTransform{glm::vec3{ 0.0f, -16.0f,  16.0f}, 0.3f});
    return MppState::Play;
}

static MppState Quit()
{
    return MppState::Quit;
}

bool MppStateTick()
{
    ticks.Tick();
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            return false;
        }
    }
    renderer.BeginFrame(window);
    switch (state)
    {
    case MppState::Init:
        state = Init();
        break;
    case MppState::Play:
        state = Play();
        break;
    case MppState::Quit:
        state = Quit();
        return false;
    }
    renderer.EndFrame();
    return true;
}