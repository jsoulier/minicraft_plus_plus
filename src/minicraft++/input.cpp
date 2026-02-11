#include <SDL3/SDL.h>

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include <minicraft++/assert.hpp>
#include <minicraft++/console.hpp>
#include <minicraft++/input.hpp>
#include <minicraft++/log.hpp>

static constexpr int kAction = SDL_SCANCODE_SPACE;
static constexpr int kDrop = SDL_SCANCODE_Q;
static constexpr int kInteract = SDL_SCANCODE_E;
static constexpr int kConsole = SDLK_SLASH;
static constexpr int kUp = SDL_SCANCODE_W;
static constexpr int kDown = SDL_SCANCODE_S;
static constexpr int kUpArrow = SDL_SCANCODE_UP;
static constexpr int kDownArrow = SDL_SCANCODE_DOWN;
static constexpr int kLeftArrow = SDL_SCANCODE_LEFT;
static constexpr int kRightArrow = SDL_SCANCODE_RIGHT;
static constexpr int kLeft = SDL_SCANCODE_A;
static constexpr int kRight = SDL_SCANCODE_D;
static constexpr int kExit = SDL_SCANCODE_ESCAPE;
static constexpr int kEnter = SDL_SCANCODE_RETURN;
static constexpr int kBackspace = SDL_SCANCODE_BACKSPACE;

static std::vector<std::weak_ptr<MppInputHandler>> handlers;
static std::shared_ptr<MppInputHandler> console = std::make_shared<MppConsole>();

void MppInputHandler::OnInteract()
{
    MppInputRemoveHandler(this);
}

void MppInputHandler::OnExit()
{
    MppInputRemoveHandler(this);
}

void MppInputQuit()
{
    console = nullptr;
}

void MppInputAddHandler(const std::shared_ptr<MppInputHandler>& handler)
{
    MppAssert(handler);
    handlers.push_back(handler);
    handler->OnGainFocus();
}

void MppInputRemoveHandler(MppInputHandler* handler)
{
    MppAssert(handler);
    handler->OnLoseFocus();
    int numErased = std::erase_if(handlers, [handler](const std::weak_ptr<MppInputHandler>& other)
    {
        return handler == other.lock().get();
    });
    MppAssert(numErased == 1);
}

static std::shared_ptr<MppInputHandler> GetHandler()
{
    if (handlers.empty())
    {
        return nullptr;
    }
    std::shared_ptr<MppInputHandler> handler = handlers.back().lock();
    while (!handler && !handlers.empty())
    {
        handlers.pop_back();
        handler = handlers.back().lock();
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
        case kInteract:
            handler->OnInteract();
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
            handler->OnExit();
            break;
        }
        if (event->key.key == SDLK_SLASH && handler != console)
        {
            MppInputAddHandler(console);
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
