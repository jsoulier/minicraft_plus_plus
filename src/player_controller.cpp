#include <SDL3/SDL.h>

#include "player_controller.hpp"

MppPlayerController::MppPlayerController(MppMobEntity& entity)
    : MppController(entity)
{
}

void MppPlayerController::GetMovement(float& dx, float& dy)
{
    // TODO: gamepad
    // TODO: rebinding
    const bool* keys = SDL_GetKeyboardState(nullptr);
    dx += keys[SDL_SCANCODE_D];
    dx -= keys[SDL_SCANCODE_A];
    dy += keys[SDL_SCANCODE_S];
    dy -= keys[SDL_SCANCODE_W];
}