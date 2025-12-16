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
        LayerPhysics,
        LayerCount,
    };

    MppRenderer();
    bool Init();
    void Quit();
    void Update(float dt);
    void SetCamera(float x, float y, float size);
    const MppCamera& GetCamera() const;
    void Draw(MppSprite sprite, float x, float y, Layer layer);
    void DrawRect(int color, float x, float y, float width, float height, Layer layer);
    void DrawLine(int color, float x1, float y1, float x2, float y2, Layer layer);

private:
    struct Sprite
    {
        bool operator<(const Sprite& other) const;

        MppSprite Sprite;
        float X;
        float Y;
    };

    struct Quad
    {
        int Color;
        float X;
        float Y;
        float Width;
        float Height;
    };

    struct Line
    {
        int Color;
        float X1;
        float Y1;
        float X2;
        float Y2;
    };

    struct Commands
    {
        std::vector<Sprite> Sprites;
        std::vector<Quad> Quads;
        std::vector<Line> Lines;
    };

    void Draw(const Sprite& sprite);

    SDL_Window* Window;
    SDL_Renderer* Renderer;
    SDL_Surface* Spritesheet;
    std::unordered_map<size_t, SDL_Palette*> SpritePalettes;
    std::unordered_map<size_t, SDL_Surface*> SpriteSurfaces;
    std::unordered_map<size_t, SDL_Texture*> SpriteTextures;
    std::array<Commands, LayerCount> LayerCommands;
    MppCamera Camera;
};