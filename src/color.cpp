#include <SDL3/SDL.h>

#include "color.hpp"

SDL_Color MppColorGet(int inColor)
{
    int red = (inColor / 100) % 10;
    int green = (inColor / 10) % 10;
    int blue = inColor % 10;
    SDL_assert(red >= 0 && red <= 9);
    SDL_assert(green >= 0 && green <= 9);
    SDL_assert(blue >= 0 && blue <= 9);
    SDL_Color color;
    color.r = red * 255 / 9;
    color.g = green * 255 / 9;
    color.b = blue * 255 / 9;
    color.a = 255;
    return color;
}