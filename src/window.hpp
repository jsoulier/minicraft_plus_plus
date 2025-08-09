#pragma once

#include <cassert>
#include <cstdint>
#include <string_view>

using MppColor = uint64_t;

static constexpr MppColor kBlack = 0;
static constexpr MppColor kGray = 444;
static constexpr MppColor kRed = 900;
static constexpr MppColor kGreen = 90;
static constexpr MppColor kBlue = 9;
static constexpr MppColor kYellow = 990;
static constexpr MppColor kCyan = 99;
static constexpr MppColor kMagenta = 909;
static constexpr MppColor kWhite = 999;

enum MppSpriteSize : uint64_t
{
    MppSpriteSize2,
    MppSpriteSize4,
    MppSpriteSize8,
    MppSpriteSize16,
    MppSpriteSizeCount,
};

/*
 * 00-09: color1 (10 bits)
 * 10-19: color2 (10 bits)
 * 20-29: color3 (10 bits)
 * 30-39: color4 (10 bits)
 * 40-49: color5 (10 bits)
 * 50-55: x (6 bits)
 * 56-61: y (6 bits)
 * 62-63: size (2 bits)
 */
using MppSprite = uint64_t;

constexpr MppSprite MppCreateSprite(
    MppColor color1,
    MppColor color2,
    MppColor color3,
    MppColor color4,
    MppColor color5,
    uint64_t x,
    uint64_t y,
    MppSpriteSize size)
{
    assert(color1 < 1024);
    assert(color2 < 1024);
    assert(color3 < 1024);
    assert(color4 < 1024);
    assert(color5 < 1024);
    assert(x < 64);
    assert(y < 64);
    assert(size < MppSpriteSizeCount);
    MppSprite sprite = 0;
    sprite |= color1 << 0;
    sprite |= color2 << 10;
    sprite |= color3 << 20;
    sprite |= color4 << 30;
    sprite |= color5 << 40;
    sprite |= x << 50;
    sprite |= y << 56;
    sprite |= size << 62;
    return sprite;
}

bool MppCreateWindow();
void MppDestroyWindow();
void MppClearWindow();
void MppPresentWindow();
void MppDraw(MppSprite sprite, float x, float y);
void MppDraw(const std::string_view& text, float x, float y, MppColor color, int size);