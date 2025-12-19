#include <SDL3/SDL.h>

#include <array>

#include "mob_entity.hpp"
#include "player_controller.hpp"

enum ActionType
{
    ActionTypeUp,
    ActionTypeDown,
    ActionTypeLeft,
    ActionTypeRight,
    ActionTypeInventory,
    ActionTypeCount,
};

static constexpr SDL_Scancode kScancodes[] =
{
    /* Up */ SDL_SCANCODE_W,
    /* Down */ SDL_SCANCODE_S,
    /* Left */ SDL_SCANCODE_A,
    /* Right */ SDL_SCANCODE_D,
    /* Inventory */ SDL_SCANCODE_E,
};

struct Action
{
    bool Held;
    bool Down;

    void Update(ActionType type)
    {
        const bool* keys = SDL_GetKeyboardState(nullptr);
        SDL_Scancode scancode = kScancodes[type];
        Down = keys[scancode] && !Held;
        Held = keys[scancode];
    }
};

static std::array<Action, SDL_SCANCODE_COUNT> actions;

MppPlayerController::MppPlayerController(MppMobEntity& entity)
    : MppController(entity)
{
}

void MppPlayerController::Update(MppLevel& level, int ticks)
{
    for (int i = 0; i < ActionTypeCount; i++)
    {
        actions[i].Update(ActionType(i));
    }
    if (!Entity.GetDrawInventory())
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
    if (actions[ActionTypeInventory].Down)
    {
        Entity.SetDrawInventory(!Entity.GetDrawInventory());
    }
}
