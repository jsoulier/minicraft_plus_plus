#include <SDL3/SDL.h>

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "assert.hpp"
#include "console.hpp"
#include "input.hpp"
#include "log.hpp"

static constexpr int kAction = SDL_SCANCODE_SPACE;
static constexpr int kDrop = SDL_SCANCODE_Q;
static constexpr int kInventory = SDL_SCANCODE_E;
static constexpr int kConsole = SDLK_SLASH;
static constexpr int kUp = SDL_SCANCODE_W;
static constexpr int kDown = SDL_SCANCODE_S;
static constexpr int kUpArrow = SDL_SCANCODE_UP;
static constexpr int kDownArrow = SDL_SCANCODE_DOWN;
static constexpr int kLeftArrow = SDL_SCANCODE_LEFT;
static constexpr int kRightArrow = SDL_SCANCODE_RIGHT;
static constexpr int kLeft = SDL_SCANCODE_A;
static constexpr int kRight = SDL_SCANCODE_D;
static constexpr int kCrouch = SDL_SCANCODE_LSHIFT;
static constexpr int kExit = SDL_SCANCODE_ESCAPE;
static constexpr int kEnter = SDL_SCANCODE_RETURN;
static constexpr int kBackspace = SDL_SCANCODE_BACKSPACE;

static std::weak_ptr<MppInputHandler> player;
static std::weak_ptr<MppInputHandler> interaction;
static std::shared_ptr<MppInputHandler> console = std::make_shared<MppConsole>();

void MppInputQuit()
{
    MppInputResetInteraction();
    MppInputResetPlayer();
}

void MppInputSetPlayer(const std::shared_ptr<MppInputHandler>& handler)
{
    MppAssert(handler);
    MppInputResetPlayer();
    player = handler;
    handler->OnGainFocus();
}

void MppInputSetInteraction(const std::shared_ptr<MppInputHandler>& handler)
{
    MppAssert(handler);
    MppInputResetInteraction();
    interaction = handler;
    handler->OnGainFocus();
}

void MppInputResetPlayer()
{
    std::shared_ptr<MppInputHandler> handler = player.lock();
    if (handler)
    {
        handler->OnLoseFocus();
        player.reset();
    }
}

void MppInputResetInteraction()
{
    std::shared_ptr<MppInputHandler> handler = interaction.lock();
    if (handler)
    {
        handler->OnLoseFocus();
        interaction.reset();
    }
}

static std::shared_ptr<MppInputHandler> GetHandler()
{
    std::shared_ptr<MppInputHandler> handler = interaction.lock();
    if (!handler)
    {
        handler = player.lock();
    }
    if (!handler)
    {
        MppLog("Missing input handler");
    }
    return handler;
}

void MppInputUpdate(uint64_t ticks)
{
    std::shared_ptr<MppInputHandler> handler = GetHandler();
    if (!handler)
    {
        return;
    }
    handler->OnUpdate(ticks);
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
    if (keys[kCrouch])
    {
        handler->OnHeldCrouch();
    }
}

void MppInputRender()
{
    std::shared_ptr<MppInputHandler> handler = GetHandler();
    if (handler)
    {
        handler->OnRender();
    }
}

void MppInputHandle(SDL_Event* event)
{
    std::shared_ptr<MppInputHandler> handler = GetHandler();
    if (!handler)
    {
        return;
    }
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        switch (event->key.scancode)
        {
        case kAction:
            handler->OnAction();
            break;
        case kDrop:
            handler->OnDrop();
            break;
        case kInventory:
            handler->OnInventory();
            break;
        case kUp:
            handler->OnUp();
            break;
        case kDown:
            handler->OnDown();
            break;
        case kUpArrow:
            handler->OnUpArrow();
            break;
        case kDownArrow:
            handler->OnDownArrow();
            break;
        case kLeftArrow:
            handler->OnLeftArrow();
            break;
        case kRightArrow:
            handler->OnRightArrow();
            break;
        case kLeft:
            handler->OnLeft();
            break;
        case kRight:
            handler->OnRight();
            break;
        case kEnter:
            handler->OnEnter();
            break;
        case kBackspace:
            handler->OnBackspace();
            break;
        case kExit:
            MppInputResetInteraction();
            break;
        }
        if (event->key.key == SDLK_SLASH && handler != console)
        {
            MppInputSetInteraction(console);
        }
    }
    else if (event->type == SDL_EVENT_TEXT_INPUT)
    {
        char character = event->text.text[0];
        if (character <= 127)
        {
            handler->OnTextInput(character);
        }
        else
        {
            MppLog("Unhandled text input: %s", event->text.text);
        }
    }
}
