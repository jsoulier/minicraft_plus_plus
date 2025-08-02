#include <SDL3/SDL.h>

#include "entity.hpp"
#include "input.hpp"
#include "log.hpp"
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
static MppInputType inputType = MppInputType::KeyboardMouse;
static std::shared_ptr<MppEntity> player;

bool MppStateInit()
{
    SDL_SetAppMetadata(kAppName, nullptr, nullptr);
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
    if (!window.Init())
    {
        MppLog("Failed to initialize window");
        return false;
    }
    if (!renderer.Init(window))
    {
        MppLog("Failed to initialize renderer");
        return false;
    }
    ticks = MppTicks{0};
    state = MppState::Init;
    player = MppEntity::Create(MppEntityType::Player);
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
    player->Update(ticks);
    renderer.Update(player->GetTransform(), ticks);
    renderer.Draw(MppModelDefault, MppTransform{glm::vec3{ 0.0f,   0.0f, -16.0f}, 0.0f});
    renderer.Draw(MppModelDefault, MppTransform{glm::vec3{ 32.0f,  0.0f, -16.0f}, 0.0f});
    renderer.Draw(MppModelDefault, MppTransform{glm::vec3{-32.0f,  0.0f, -16.0f}, 0.0f});
    renderer.Draw(MppModelDefault, MppTransform{glm::vec3{ 0.0f, -16.0f,  16.0f}, 0.3f});
    renderer.Draw(MppModelDefault, player->GetTransform());
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

MppState MppStateGet()
{
    return state;
}

MppInputType MppStateGetInputType()
{
    return inputType;
}

float MppStateGetInput(MppInput input)
{
    /* TODO: */
    const bool* keys = SDL_GetKeyboardState(nullptr);
    switch (input)
    {
    case MppInput::MoveNorth:
        return keys[SDL_SCANCODE_W];
    case MppInput::MoveSouth:
        return keys[SDL_SCANCODE_S];
    case MppInput::MoveEast:
        return keys[SDL_SCANCODE_D];
    case MppInput::MoveWest:
        return keys[SDL_SCANCODE_A];
    }
    return 0.0f;
}