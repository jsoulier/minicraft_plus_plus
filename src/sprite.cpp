#include <SDL3/SDL.h>

#include <cstddef>
#include <cstdint>
#include <limits>

#include "assert.hpp"
#include "color.hpp"
#include "sprite.hpp"

enum Size
{
    Size8,
    Size16,
    SizeCount,
};

static constexpr uint64_t kInvalid = std::numeric_limits<uint64_t>::max();
static constexpr int kSizes[SizeCount] = {8, 16};
static constexpr int kAlignment = 16;

MppSprite::MppSprite()
    : Value{kInvalid}
{
}

MppSprite::MppSprite(int color, char character)
    : MppSprite(color, 0, 0, 0, 0, character % 16 + 16, character / 16, 8)
{
}

MppSprite::MppSprite(int color)
    : MppSprite(color, 0, 0, 0, 0, 0, 0, 16)
{
}

MppSprite::MppSprite(int c1, int c2, int c3, int c4, int c5, int x, int y, int s)
    : Value{0}
{
    static_assert(kSizes[Size8] == 8);
    static_assert(kSizes[Size16] == 16);
    Size size;
    switch (s)
    {
    case 8:
        size = Size8;
        break;
    case 16:
        size = Size16;
        break;
    default:
        size = Size8;
        MppAssert(false);
    }
    MppAssert(c1 < 1024);
    MppAssert(c2 < 1024);
    MppAssert(c3 < 1024);
    MppAssert(c4 < 1024);
    MppAssert(c5 < 1024);
    MppAssert(x < 64);
    MppAssert(y < 64);
    MppAssert(size < 2);
    Value |= static_cast<uint64_t>(c1) << 0;
    Value |= static_cast<uint64_t>(c2) << 10;
    Value |= static_cast<uint64_t>(c3) << 20;
    Value |= static_cast<uint64_t>(c4) << 30;
    Value |= static_cast<uint64_t>(c5) << 40;
    Value |= static_cast<uint64_t>(x) << 50;
    Value |= static_cast<uint64_t>(y) << 56;
    Value |= static_cast<uint64_t>(size) << 62;
}

size_t MppSprite::GetPaletteKey() const
{
    return Value & 0x0003ffffffffffffull;
}

size_t MppSprite::GetSurfaceKey() const
{
    return Value & 0x3fff000000000000ull;
}

size_t MppSprite::GetTextureKey() const
{
    return Value;
}

SDL_Color MppSprite::GetColor1() const
{
    return MppColorGet((Value >> 0) & 0x3FF);
}

SDL_Color MppSprite::GetColor2() const
{
    return MppColorGet((Value >> 10) & 0x3FF);
}

SDL_Color MppSprite::GetColor3() const
{
    return MppColorGet((Value >> 20) & 0x3FF);
}

SDL_Color MppSprite::GetColor4() const
{
    return MppColorGet((Value >> 30) & 0x3FF);
}

SDL_Color MppSprite::GetColor5() const
{
    return MppColorGet((Value >> 40) & 0x3FF);
}

int MppSprite::GetX() const
{
    return ((Value >> 50) & 0x3F) * kAlignment;
}

int MppSprite::GetY() const
{
    return ((Value >> 56) & 0x3F) * kAlignment;
}

int MppSprite::GetSize() const
{
    return kSizes[(Value >> 62) & 0x3];
}

bool MppSprite::operator==(const MppSprite other) const
{
    return Value == other.Value;
}

bool MppSprite::operator<(const MppSprite other) const
{
    return GetTextureKey() < other.GetTextureKey();
}

bool MppSprite::IsValid() const
{
    return !operator==(MppSprite{});
}
