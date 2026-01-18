#pragma once

#include <SDL3/SDL.h>

#include <cstdint>
#include <cstddef>

class MppSprite
{
public:
    MppSprite();
    MppSprite(int color, char character);
    MppSprite(int color);
    MppSprite(int c1, int c2, int c3, int c4, int c5, int x, int y, int size);
    size_t GetPaletteKey() const;
    size_t GetSurfaceKey() const;
    size_t GetTextureKey() const;
    SDL_Color GetColor1() const;
    SDL_Color GetColor2() const;
    SDL_Color GetColor3() const;
    SDL_Color GetColor4() const;
    SDL_Color GetColor5() const;
    int GetX() const;
    int GetY() const;
    int GetSize() const;
    bool operator==(const MppSprite other) const;
    bool operator<(const MppSprite other) const;
    bool IsValid() const;

private:
    uint64_t Value;
};
