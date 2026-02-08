#pragma once

#include <SDL3/SDL.h>

#include <cstdint>
#include <cstddef>
#include <utility>

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

class MppSpriteAnimation
{
private:
    static constexpr int kMaxPoses = 2;

public:
    MppSpriteAnimation();
    void Update(int pose, int dx, int dy, uint64_t ticks);
    void Update(int pose, int dx, int dy);
    void SetPose(int pose, int x, int y);
    void SetTickRate(int rate);
    int GetX() const;
    int GetY() const;
    bool GetFlip() const;

private:
    int Poses[kMaxPoses][2];
    int TickRate;
    bool Tick;
    int X;
    int Y;
    bool Flip;
};
