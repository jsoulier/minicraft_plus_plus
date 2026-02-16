#include <SDL3/SDL.h>

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include <minicraft++/assert.hpp>
#include <minicraft++/console.hpp>
#include <minicraft++/input.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/renderer.hpp>

static constexpr int kAction = SDL_SCANCODE_SPACE;
static constexpr int kDrop = SDL_SCANCODE_Q;
static constexpr int kInteract = SDL_SCANCODE_E;
static constexpr int kDismount = SDL_SCANCODE_LSHIFT;
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

void MppInputHandler::OnInputInteract()
{
    MppInputRemoveHandler(this);
}

void MppInputHandler::OnInputExit()
{
    MppInputRemoveHandler(this);
}

void MppInputAddHandler(const std::shared_ptr<MppInputHandler>& handler)
{
    MppAssert(handler);
    for (std::weak_ptr<MppInputHandler>& other : handlers)
    {
        MppAssert(handler != other.lock());
    }
    handlers.push_back(handler);
    handler->OnInputGainFocus();
}

void MppInputRemoveHandler(MppInputHandler* handler)
{
    MppAssert(handler);
    handler->OnInputLoseFocus();
    int count = std::erase_if(handlers, [handler](const std::weak_ptr<MppInputHandler>& other)
    {
        return handler == other.lock().get();
    });
    MppAssert(count == 1);
}

static std::shared_ptr<MppInputHandler> GetHandler()
{
    if (handlers.empty())
    {
        return nullptr;
    }
    std::shared_ptr<MppInputHandler> handler = handlers.back().lock();
    while (!handler)
    {
        handlers.pop_back();
        if (handlers.empty())
        {
            break;
        }
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
    handler->OnInputUpdate(ticks);
    const bool* keys = SDL_GetKeyboardState(nullptr);
    if (keys[kUp])
    {
        handler->OnInputHeldUp();
    }
    if (keys[kDown])
    {
        handler->OnInputHeldDown();
    }
    if (keys[kLeft])
    {
        handler->OnInputHeldLeft();
    }
    if (keys[kRight])
    {
        handler->OnInputHeldRight();
    }
}

void MppInputRender()
{
    for (int i = 0; i < handlers.size(); i++)
    {
        if (i > 5)
        {
            MppLog("Ran out of renderer layers");
            break;
        }
        const std::weak_ptr<MppInputHandler> handler = handlers[i];
        if (!handler.expired())
        {
            static constexpr MppRendererLayer kMenu = MppRendererLayerMenu1;
            static constexpr MppRendererLayer kTopMenu = MppRendererLayerTopMenu1;
            MppRendererLayerOverride override1{kMenu, MppRendererLayer(kMenu + i * 2)};
            MppRendererLayerOverride override2{kTopMenu, MppRendererLayer(kTopMenu + i * 2)};
            handler.lock()->OnInputRender();
        }
    }
}

void MppInputHandle(SDL_Event* event)
{
    std::shared_ptr<MppInputHandler> handler = GetHandler();
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.key == SDLK_SLASH && handler != console)
        {
            MppInputAddHandler(console);
        }
    }
    if (!handler)
    {
        return;
    }
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        switch (event->key.scancode)
        {
        case kAction:
            handler->OnInputAction();
            break;
        case kDrop:
            handler->OnInputDrop();
            break;
        case kInteract:
            handler->OnInputInteract();
            break;
        case kDismount:
            handler->OnInputDismount();
            break;
        case kUp:
            handler->OnInputUp();
            break;
        case kDown:
            handler->OnInputDown();
            break;
        case kUpArrow:
            handler->OnInputUpArrow();
            break;
        case kDownArrow:
            handler->OnInputDownArrow();
            break;
        case kLeftArrow:
            handler->OnInputLeftArrow();
            break;
        case kRightArrow:
            handler->OnInputRightArrow();
            break;
        case kLeft:
            handler->OnInputLeft();
            break;
        case kRight:
            handler->OnInputRight();
            break;
        case kEnter:
            handler->OnInputEnter();
            break;
        case kBackspace:
            handler->OnInputBackspace();
            break;
        case kExit:
            handler->OnInputExit();
            break;
        }
    }
    else if (event->type == SDL_EVENT_TEXT_INPUT)
    {
        char character = event->text.text[0];
        if (character <= 127)
        {
            handler->OnInputText(character);
        }
        else
        {
            MppLog("Unhandled text input: %s", event->text.text);
        }
    }
}
