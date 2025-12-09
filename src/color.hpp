#pragma once

#include <SDL3/SDL.h>

static constexpr int kMppColorBlack = 0;
static constexpr int kMppColorRed = 900;
static constexpr int kMppColorGreen = 90;
static constexpr int kMppColorBlue = 9;
static constexpr int kMppColorYellow = 990;
static constexpr int kMppColorMagenta = 909;
static constexpr int kMppColorCyan = 99;
static constexpr int kMppColorWhite = 999;

SDL_Color MppColorGet(int inColor);