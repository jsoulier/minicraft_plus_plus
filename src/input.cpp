#include <SDL3/SDL.h>

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "assert.hpp"
#include "input.hpp"
#include "log.hpp"

static constexpr int kAction = SDL_SCANCODE_SPACE;
static constexpr int kUp = SDL_SCANCODE_W;
static constexpr int kDown = SDL_SCANCODE_S;
static constexpr int kLeft = SDL_SCANCODE_A;
static constexpr int kRight = SDL_SCANCODE_D;
static constexpr int kExit = SDL_SCANCODE_ESCAPE;

static std::weak_ptr<MppInputHandler> player;
static std::weak_ptr<MppInputHandler> interaction;

void MppInputSetPlayer(const std::shared_ptr<MppInputHandler>& handler)
{
    MppAssert(player.expired());
    MppAssert(handler);
    player = handler;
}

void MppInputSetInteraction(const std::shared_ptr<MppInputHandler>& handler)
{
    MppAssert(interaction.expired());
    MppAssert(handler);
    interaction = handler;
}

void MppInputUpdate(uint64_t ticks)
{
    std::shared_ptr<MppInputHandler> handler = interaction.lock();
    if (!handler)
    {
        handler = player.lock();
    }
    if (!handler)
    {
        MppLog("Missing input handler");
        return;
    }
    const bool* keys = SDL_GetKeyboardState(nullptr);
    if (keys[kAction])
    {
        handler->OnAction();
    }
    if (keys[kUp])
    {
        handler->OnHeldUp();
    }
    if (keys[kDown])
    {
        handler->OnHeldDown();
    }
    if (keys[kLeft])
    {
        handler->OnHeldLeft();
    }
    if (keys[kRight])
    {
        handler->OnHeldRight();
    }
    if (keys[kExit])
    {
        interaction.reset();
    }
}
