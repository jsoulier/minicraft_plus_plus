#include <SDL3/SDL.h>

#include <array>

#include "inventory.hpp"
#include "menu_list.hpp"
#include "mob_entity.hpp"
#include "player_controller.hpp"
#include "renderer.hpp"

enum ActionType
{
    ActionTypeMoveUp,
    ActionTypeMoveDown,
    ActionTypeMoveLeft,
    ActionTypeMoveRight,
    ActionTypeMenuUp,
    ActionTypeMenuDown,
    ActionTypeMenuLeft,
    ActionTypeMenuRight,
    ActionTypeToggleInventory,
    ActionTypeCount,
};

static constexpr SDL_Scancode kScancodes[] =
{
    /* MoveUp */ SDL_SCANCODE_W,
    /* MoveDown */ SDL_SCANCODE_S,
    /* MoveLeft */ SDL_SCANCODE_A,
    /* MoveRight */ SDL_SCANCODE_D,
    /* MenuUp */ SDL_SCANCODE_UP,
    /* MenuDown */ SDL_SCANCODE_DOWN,
    /* MenuLeft */ SDL_SCANCODE_LEFT,
    /* MenuRight */ SDL_SCANCODE_RIGHT,
    /* ToggleInventory */ SDL_SCANCODE_E,
};

struct Action
{
    void Update(ActionType type)
    {
        const bool* keys = SDL_GetKeyboardState(nullptr);
        SDL_Scancode scancode = kScancodes[type];
        Down = keys[scancode] && !Held;
        Held = keys[scancode];
    }

    bool Held;
    bool Down;
};

static std::array<Action, SDL_SCANCODE_COUNT> actions;

MppPlayerController::MppPlayerController(MppMobEntity& entity)
    : MppController(entity)
    , MenuList{}
{
}

void MppPlayerController::Update(MppLevel& level, MppRenderer& renderer, int ticks)
{
    for (int i = 0; i < ActionTypeCount; i++)
    {
        actions[i].Update(ActionType(i));
    }
    if (actions[ActionTypeToggleInventory].Down)
    {
        Toggle(std::dynamic_pointer_cast<MppMenuList>(Entity.GetInventory()));
    }
    if (!MenuList)
    {
        int dx = 0;
        int dy = 0;
        dy -= actions[ActionTypeMoveUp].Held;
        dy += actions[ActionTypeMoveDown].Held;
        dx -= actions[ActionTypeMoveLeft].Held;
        dx += actions[ActionTypeMoveRight].Held;
        if (dx || dy)
        {
            Entity.Move(level, dx, dy, ticks);
        }
    }
    else
    {
        if (actions[ActionTypeMenuUp].Down)
        {
            MenuList->Up();
        }
        else if (actions[ActionTypeMenuDown].Down)
        {
            MenuList->Down();
        }
        MenuList->Draw(renderer);
    }
}

void MppPlayerController::Toggle(const std::shared_ptr<MppMenuList>& menuList)
{
    if (MenuList)
    {
        MenuList.reset();
    }
    else
    {
        MenuList = menuList;
    }
}
