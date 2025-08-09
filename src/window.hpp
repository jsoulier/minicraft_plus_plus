#pragma once

#include <cassert>
#include <cstdint>
#include <string_view>

static constexpr int kBlack = 0;
static constexpr int kGray = 444;
static constexpr int kRed = 900;
static constexpr int kGreen = 90;
static constexpr int kBlue = 9;
static constexpr int kYellow = 990;
static constexpr int kCyan = 99;
static constexpr int kMagenta = 909;
static constexpr int kWhite = 999;

enum MppSpriteSize
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
    int colour1,
    int colour2,
    int colour3,
    int colour4,
    int colour5,
    uint64_t x,
    uint64_t y,
    MppSpriteSize size)
{
    assert(colour1 < 1024);
    assert(colour2 < 1024);
    assert(colour3 < 1024);
    assert(colour4 < 1024);
    assert(colour5 < 1024);
    assert(x < 64);
    assert(y < 64);
    assert(size < MppSpriteSizeCount);
    MppSprite sprite = 0;
    sprite |= static_cast<uint64_t>(colour1) << 0;
    sprite |= static_cast<uint64_t>(colour2) << 10;
    sprite |= static_cast<uint64_t>(colour3) << 20;
    sprite |= static_cast<uint64_t>(colour4) << 30;
    sprite |= static_cast<uint64_t>(colour5) << 40;
    sprite |= x << 50;
    sprite |= y << 56;
    sprite |= static_cast<uint64_t>(size) << 62;
    return sprite;
}

bool MppCreateWindow();
void MppDestroyWindow();
void MppClearWindow();
void MppPresentWindow();
void MppDraw(MppSprite sprite, float x, float y);
void MppDraw(const std::string_view& text, float x, float y, int colour, int size);
void MppDraw(const std::string_view& name, int colour1, int colour2, int colour3, int colour4, int colour5);