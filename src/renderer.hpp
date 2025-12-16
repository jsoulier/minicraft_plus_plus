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
        LayerScreen,
        LayerDebugPhysics,
        LayerDebug,
        LayerCount,
    };

    MppRenderer();
    bool Init();
    void Quit();
    void Update(float dt);
    void SetCamera(float x, float y, float size);
    const MppCamera& GetCamera() const;
    void Draw(MppSprite sprite, float x, float y, Layer layer);

private:
    struct Sprite
    {
        bool operator<(const Sprite& other) const;

        MppSprite Sprite;
        float X;
        float Y;
    };

    void Draw(const Sprite& sprite);

    SDL_Window* Window;
    SDL_Renderer* Renderer;
    SDL_Surface* Spritesheet;
    std::unordered_map<size_t, SDL_Palette*> SpritePalettes;
    std::unordered_map<size_t, SDL_Surface*> SpriteSurfaces;
    std::unordered_map<size_t, SDL_Texture*> SpriteTextures;
    std::array<std::vector<Sprite>, LayerCount> Sprites;
    MppCamera Camera;
};