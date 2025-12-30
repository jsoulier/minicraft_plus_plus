#pragma once

#include <SDL3/SDL.h>

#include <array>
#include <cstddef>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "camera.hpp"
#include "color.hpp"
#include "sprite.hpp"

class MppRenderer
{
public:
    enum Layer
    {
        LayerBottomTile,
        LayerTile,
        LayerTopTile,
        LayerMobEntity,
        LayerMobEntityOverlay,
        LayerPhysics,
        LayerScreenDebug,
        LayerScreen,
        LayerScreenContent,
        LayerCount,
    };

    MppRenderer();
    bool Init();
    void Quit();
    void Update(int ticks);
    void SetCamera(int x, int y, int size);
    const MppCamera& GetCamera() const;
    void Draw(MppSprite sprite, int x, int y, bool flip, Layer layer);
    void DrawRect(int color, int x, int y, int width, int height, Layer layer);
    void DrawLine(int color, int x1, int y1, int x2, int y2, Layer layer);

private:
    struct Sprite
    {
        bool operator<(const Sprite& other) const;

        MppSprite Sprite;
        int X;
        int Y;
        bool Flip;
    };

    struct Quad
    {
        int Color;
        int X;
        int Y;
        int Width;
        int Height;
    };

    struct Line
    {
        int Color;
        int X1;
        int Y1;
        int X2;
        int Y2;
    };

    struct Commands
    {
        std::vector<Sprite> Sprites;
        std::vector<Quad> Quads;
        std::vector<Line> Lines;
    };

    void UseCamera(int& x, int& y, Layer layer);
    void Draw(Sprite& sprite, Layer layer);

    SDL_Window* Window;
    SDL_Renderer* Renderer;
    SDL_Surface* Spritesheet;
    std::unordered_map<size_t, SDL_Palette*> SpritePalettes;
    std::unordered_map<size_t, SDL_Surface*> SpriteSurfaces;
    std::unordered_map<size_t, SDL_Texture*> SpriteTextures;
    std::array<Commands, LayerCount> LayerCommands;
    MppCamera Camera;
};