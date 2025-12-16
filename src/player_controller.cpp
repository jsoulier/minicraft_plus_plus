#include <SDL3/SDL.h>

#include "mob_entity.hpp"
#include "player_controller.hpp"

MppPlayerController::MppPlayerController(MppMobEntity& entity)
    : MppController(entity)
{
}

void MppPlayerController::Update(MppLevel& level, int ticks)
{
    int dx = 0;
    int dy = 0;
    // TODO: gamepad, rebinding, etc
    const bool* keys = SDL_GetKeyboardState(nullptr);
    dx += keys[SDL_SCANCODE_D];
    dx -= keys[SDL_SCANCODE_A];
    dy += keys[SDL_SCANCODE_S];
    dy -= keys[SDL_SCANCODE_W];
    Entity.Move(level, dx, dy, ticks);
}