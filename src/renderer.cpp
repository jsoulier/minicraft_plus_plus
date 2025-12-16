#include <SDL3/SDL.h>

#include <algorithm>
#include <filesystem>
#include <format>
#include <string>
#include <vector>

#include "color.hpp"
#include "renderer.hpp"
#include "sprite.hpp"
#include "version.hpp"

static constexpr int kWidth = 256;
static constexpr int kHeight = 144;
static const std::filesystem::path kBasePath = SDL_GetBasePath();
static const std::filesystem::path kSpritesheet = kBasePath / "spritesheet.png";

MppRenderer::MppRenderer()
    : Window{nullptr}
    , Renderer{nullptr}
    , Spritesheet{nullptr}
{
    Camera.SetSize(kWidth, kHeight);
}

bool MppRenderer::Init()
{
    int width = kWidth * 4;
    int height = kHeight * 4;
    if (!SDL_CreateWindowAndRenderer("Minicraft++", width, height, 0, &Window, &Renderer))
    {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
        return false;
    }
    Spritesheet = SDL_LoadPNG(kSpritesheet.string().data());
    if (!Spritesheet)
    {
        SDL_Log("Failed to create spritesheet: %s", SDL_GetError());
        return false;
    }
    SDL_SetWindowResizable(Window, true);
    SDL_FlashWindow(Window, SDL_FLASH_BRIEFLY);
    SDL_SetRenderLogicalPresentation(Renderer, kWidth, kHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderVSync(Renderer, true);
    SDL_SetDefaultTextureScaleMode(Renderer, SDL_SCALEMODE_NEAREST);
    return true;
}

void MppRenderer::Quit()
{
    for (auto& [key, value] : SpritePalettes)
    {
        SDL_DestroyPalette(value);
    }
    for (auto& [key, value] : SpriteSurfaces)
    {
        SDL_DestroySurface(value);
    }
    for (auto& [key, value] : SpriteTextures)
    {
        SDL_DestroyTexture(value);
    }
    SpritePalettes.clear();
    SpriteSurfaces.clear();
    SpriteTextures.clear();
    SDL_DestroySurface(Spritesheet);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    Spritesheet = nullptr;
    Renderer = nullptr;
    Window = nullptr;
}

void MppRenderer::Update(float dt)
{
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    for (int layer = 0; layer < LayerCount; layer++)
    {
        std::vector<Sprite>& sprites = Sprites[layer];
        std::sort(sprites.begin(), sprites.end());
        for (const Sprite& sprite : sprites)
        {
            Draw(sprite);
        }
        sprites.clear();
    }
    SDL_RenderPresent(Renderer);
    Camera.Update(dt);
}

void MppRenderer::SetCamera(float x, float y, float size)
{
    Camera.SetPosition(x, y, size);
}

const MppCamera& MppRenderer::GetCamera() const
{
    return Camera;
}

void MppRenderer::Draw(MppSprite sprite, float x, float y, Layer layer)
{
    Sprites[layer].emplace_back(sprite, x, y);
}

bool MppRenderer::Sprite::operator<(const MppRenderer::Sprite& other) const
{
    return Sprite < other.Sprite;
}

void MppRenderer::Draw(const Sprite& sprite)
{
    size_t paletteKey = sprite.Sprite.GetPaletteKey();
    size_t surfaceKey = sprite.Sprite.GetSurfaceKey();
    size_t textureKey = sprite.Sprite.GetTextureKey();
    int size = sprite.Sprite.GetSize();
    auto textureIt = SpriteTextures.find(textureKey);
    if (textureIt == SpriteTextures.end())
    {
        auto paletteIt = SpritePalettes.find(paletteKey);
        if (paletteIt == SpritePalettes.end())
        {
            SDL_Palette* palette = SDL_CreatePalette(6);
            if (!palette)
            {
                SDL_Log("Failed to create palette: %s", SDL_GetError());
                return;
            }
            SDL_Color colors[6];
            colors[0] = sprite.Sprite.GetColor1();
            colors[1] = sprite.Sprite.GetColor2();
            colors[2] = sprite.Sprite.GetColor3();
            colors[3] = sprite.Sprite.GetColor4();
            colors[4] = sprite.Sprite.GetColor5();
            colors[5].a = 0;
            SDL_SetPaletteColors(palette, colors, 0, 6);
            paletteIt = SpritePalettes.emplace(paletteKey, palette).first;
        }
        auto surfaceIt = SpriteSurfaces.find(surfaceKey);
        if (surfaceIt == SpriteSurfaces.end())
        {
            SDL_Rect rect;
            rect.x = sprite.Sprite.GetX();
            rect.y = sprite.Sprite.GetY();
            rect.w = size;
            rect.h = size;
            SDL_Surface* surface = SDL_CreateSurface(size, size, SDL_PIXELFORMAT_INDEX8);
            if (!surface)
            {
                SDL_Log("Failed to create surface: %s", SDL_GetError());
                return;
            }
            SDL_BlitSurface(Spritesheet, &rect, surface, nullptr);
            surfaceIt = SpriteSurfaces.emplace(surfaceKey, surface).first;
        }
        if (!surfaceIt->second)
        {
            return;
        }
        SDL_SetSurfacePalette(surfaceIt->second, paletteIt->second);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, surfaceIt->second);
        if (!texture)
        {
            SDL_Log("Failed to create texture: %s", SDL_GetError());
            return;
        }
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        textureIt = SpriteTextures.emplace(textureKey, texture).first;
    }
    if (!textureIt->second)
    {
        return;
    }
    SDL_FRect rect;
    rect.x = sprite.X - Camera.X;
    rect.y = sprite.Y - Camera.Y;
    rect.w = size;
    rect.h = size;
    SDL_RenderTexture(Renderer, textureIt->second, nullptr, &rect);
}