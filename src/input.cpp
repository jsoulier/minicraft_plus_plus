#include <SDL3/SDL.h>

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "input.hpp"
#include "log.hpp"

static constexpr int kUp = SDL_SCANCODE_W;
static constexpr int kDown = SDL_SCANCODE_S;
static constexpr int kLeft = SDL_SCANCODE_A;
static constexpr int kRight = SDL_SCANCODE_D;

static std::vector<std::weak_ptr<MppInputHandler>> handlers;

void MppInputPush(const std::shared_ptr<MppInputHandler>& handler)
{
    handlers.push_back(handler);
}

void MppInputPop(const std::shared_ptr<MppInputHandler>& handler)
{
    std::vector<std::weak_ptr<MppInputHandler>> newHandlers;
    for (int i = 0; i < handlers.size(); i++)
    {
        if (handlers[i].lock().get() != handler.get())
        {
            handlers.push_back(handlers[i]);
        }
    }
    handlers = std::move(newHandlers);
}

void MppInputUpdate(uint64_t ticks)
{
    std::shared_ptr<MppInputHandler> handler;
    while (handlers.size())
    {
        handler = handlers.back().lock();
        if (!handler)
        {
            MppLog("Found expired input handler");
            handlers.pop_back();
        }
        else
        {
            break;
        }
    }
    if (!handler)
    {
        MppLog("No input handlers");
        return;
    }
    const bool* keys = SDL_GetKeyboardState(nullptr);
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
}
