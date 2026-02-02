#include <SDL3/SDL.h>

#include "assert.hpp"
#include "color.hpp"

SDL_Color MppColorGet(int inColor)
{
    int red = (inColor / 100) % 10;
    int green = (inColor / 10) % 10;
    int blue = inColor % 10;
    MppAssert(red >= 0 && red <= 5);
    MppAssert(green >= 0 && green <= 5);
    MppAssert(blue >= 0 && blue <= 5);
    SDL_Color color;
    color.r = red * 255 / 5;
    color.g = green * 255 / 5;
    color.b = blue * 255 / 5;
    color.a = 255;
    return color;
}