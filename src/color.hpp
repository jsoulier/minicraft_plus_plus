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

static constexpr int kMppColorDirtBrown = 840;

static constexpr int kMppColorWood1 = 640;
static constexpr int kMppColorWood2 = 750;
static constexpr int kMppColorWood3 = 860;

static constexpr int kMppColorStone1 = 444;
static constexpr int kMppColorStone2 = 111;
static constexpr int kMppColorStone3 = 666;
static constexpr int kMppColorStone4 = 555;

SDL_Color MppColorGet(int inColor);