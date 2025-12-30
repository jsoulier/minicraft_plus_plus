#include <SDL3/SDL.h>

#include <array>

#include "menu_list.hpp"
#include "mob_entity.hpp"
#include "mob_inventory.hpp"
#include "player_controller.hpp"
#include "renderer.hpp"

enum ActionType
{
    ActionTypeAction,
    ActionTypeUp,
    ActionTypeDown,
    ActionTypeLeft,
    ActionTypeRight,
    ActionTypeInventory,
    ActionTypeCount,
};

static constexpr SDL_Scancode kScancodes[ActionTypeCount][2] =
{
    /* Action */    {SDL_SCANCODE_SPACE, SDL_SCANCODE_RETURN},
    /* Up */        {SDL_SCANCODE_W, SDL_SCANCODE_UP},
    /* Down */      {SDL_SCANCODE_S, SDL_SCANCODE_DOWN},
    /* Left */      {SDL_SCANCODE_A, SDL_SCANCODE_LEFT},
    /* Right */     {SDL_SCANCODE_D, SDL_SCANCODE_RIGHT},
    /* Inventory */ {SDL_SCANCODE_E, SDL_SCANCODE_UNKNOWN},
};

struct Action
{
    void Update(ActionType type)
    {
        const bool* keys = SDL_GetKeyboardState(nullptr);
        Down = false;
        for (int i = 0; i < 2; i++)
        {
            SDL_Scancode scancode = kScancodes[type][i];
            Down |= keys[scancode] && !Held;
        }
        Held = false;
        for (int i = 0; i < 2; i++)
        {
            SDL_Scancode scancode = kScancodes[type][i];
            Held |= keys[scancode];
        }
    }

    bool Held;
    // TODO: repeat
    bool Down;
};

static std::array<Action, ActionTypeCount> actions;

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
    if (actions[ActionTypeInventory].Down)
    {
        Toggle(std::dynamic_pointer_cast<MppMenuList>(Entity.GetInventory()));
    }
    if (!MenuList)
    {
        int dx = 0;
        int dy = 0;
        dy -= actions[ActionTypeUp].Held;
        dy += actions[ActionTypeDown].Held;
        dx -= actions[ActionTypeLeft].Held;
        dx += actions[ActionTypeRight].Held;
        if (dx || dy)
        {
            Entity.Move(level, dx, dy, ticks);
        }
    }
    else
    {
        if (actions[ActionTypeAction].Down)
        {
            MenuList->Action();
        }
        else if (actions[ActionTypeDown].Down)
        {
            MenuList->Down();
        }
        else if (actions[ActionTypeUp].Down)
        {
            MenuList->Up();
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
