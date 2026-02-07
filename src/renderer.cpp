#include <SDL3/SDL.h>

#include <algorithm>
#include <filesystem>
#include <format>
#include <string>
#include <vector>

#include "assert.hpp"
#include "color.hpp"
#include "log.hpp"
#include "renderer.hpp"
#include "sprite.hpp"
#include "tile.hpp"
#include "world.hpp"

struct Sprite
{
    bool operator<(const Sprite& other) const
    {
        return Sprite < other.Sprite;
    }

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

static constexpr int kWidth = 256;
static constexpr int kHeight = 144;
static const std::filesystem::path kBasePath = SDL_GetBasePath();
static const std::filesystem::path kSpritesheet = kBasePath / "spritesheet.png";

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Surface* spritesheet;
static std::unordered_map<size_t, SDL_Palette*> palettes;
static std::unordered_map<size_t, SDL_Surface*> surfaces;
static std::unordered_map<size_t, SDL_Texture*> textures;
static std::array<Commands, MppRendererLayerCount> layers;
static int worldX;
static int worldY;
static int tileX1;
static int tileY1;
static int tileX2;
static int tileY2;

bool MppRendererInit()
{
    if (!SDL_CreateWindowAndRenderer("Minicraft++", kWidth * 4, kHeight * 4, 0, &window, &renderer))
    {
        MppLog("Failed to create window and renderer: %s", SDL_GetError());
        return false;
    }
    spritesheet = SDL_LoadPNG(kSpritesheet.string().data());
    if (!spritesheet)
    {
        MppLog("Failed to create spritesheet: %s", SDL_GetError());
        return false;
    }
    SDL_SetWindowResizable(window, true);
    SDL_FlashWindow(window, SDL_FLASH_BRIEFLY);
    SDL_SetRenderLogicalPresentation(renderer, kWidth, kHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderVSync(renderer, true);
    SDL_SetDefaultTextureScaleMode(renderer, SDL_SCALEMODE_NEAREST);
    return true;
}

void MppRendererQuit()
{
    SDL_HideWindow(window);
    for (auto& [key, value] : palettes)
    {
        SDL_DestroyPalette(value);
    }
    for (auto& [key, value] : surfaces)
    {
        SDL_DestroySurface(value);
    }
    for (auto& [key, value] : textures)
    {
        SDL_DestroyTexture(value);
    }
    palettes.clear();
    surfaces.clear();
    textures.clear();
    SDL_DestroySurface(spritesheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    spritesheet = nullptr;
    renderer = nullptr;
    window = nullptr;
}

void MppRendererMove(int x, int y, int size)
{
    MppAssert(size > 0);
    worldX = x - kWidth / 2 + size / 2;
    worldY = y - kHeight / 2 + size / 2;
}

static void Move(int& x, int& y, MppRendererLayer layer)
{
    switch (layer)
    {
    case MppRendererLayerBottomTile:
    case MppRendererLayerTile:
    case MppRendererLayerTopTile:
    case MppRendererLayerEntity:
    case MppRendererLayerEntityOverlay:
    case MppRendererLayerParticleEntity:
    case MppRendererLayerDebugPhysics:
    case MppRendererLayerDebugNavigation:
    case MppRendererLayerDebugFov:
    case MppRendererLayerDebugAction:
        x -= worldX;
        y -= worldY;
        break;
    case MppRendererLayerMenu:
    case MppRendererLayerMenuContent:
        break;
    default:
        MppAssert(false);
    }
}

static void DrawSprite(Sprite& sprite, MppRendererLayer layer)
{
    size_t paletteKey = sprite.Sprite.GetPaletteKey();
    size_t surfaceKey = sprite.Sprite.GetSurfaceKey();
    size_t textureKey = sprite.Sprite.GetTextureKey();
    int size = sprite.Sprite.GetSize();
    auto textureIt = textures.find(textureKey);
    if (textureIt == textures.end())
    {
        auto paletteIt = palettes.find(paletteKey);
        if (paletteIt == palettes.end())
        {
            SDL_Palette* palette = SDL_CreatePalette(6);
            if (!palette)
            {
                MppLog("Failed to create palette: %s", SDL_GetError());
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
            paletteIt = palettes.emplace(paletteKey, palette).first;
        }
        auto surfaceIt = surfaces.find(surfaceKey);
        if (surfaceIt == surfaces.end())
        {
            SDL_Rect rect;
            rect.x = sprite.Sprite.GetX();
            rect.y = sprite.Sprite.GetY();
            rect.w = size;
            rect.h = size;
            SDL_Surface* surface = SDL_CreateSurface(size, size, SDL_PIXELFORMAT_INDEX8);
            if (!surface)
            {
                MppLog("Failed to create surface: %s", SDL_GetError());
                return;
            }
            SDL_BlitSurface(spritesheet, &rect, surface, nullptr);
            surfaceIt = surfaces.emplace(surfaceKey, surface).first;
        }
        if (!surfaceIt->second)
        {
            return;
        }
        SDL_SetSurfacePalette(surfaceIt->second, paletteIt->second);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surfaceIt->second);
        if (!texture)
        {
            MppLog("Failed to create texture: %s", SDL_GetError());
            return;
        }
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        textureIt = textures.emplace(textureKey, texture).first;
    }
    if (!textureIt->second)
    {
        return;
    }
    Move(sprite.X, sprite.Y, layer);
    SDL_Texture* texture = textureIt->second;
    SDL_FRect rect;
    rect.x = sprite.X;
    rect.y = sprite.Y;
    rect.w = size;
    rect.h = size;
    if (sprite.Flip)
    {
        SDL_RenderTextureRotated(renderer, texture, nullptr, &rect, 180.0f, nullptr, SDL_FLIP_VERTICAL);
    }
    else
    {
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }
}

static void DrawQuad(Quad& quad, MppRendererLayer layer)
{
    Move(quad.X, quad.Y, layer);
    SDL_FRect rect;
    rect.x = quad.X;
    rect.y = quad.Y;
    rect.w = quad.Width;
    rect.h = quad.Height;
    SDL_Color color = MppColorGet(quad.Color);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

static void DrawLine(Line& line, MppRendererLayer layer)
{
    Move(line.X1, line.Y1, layer);
    Move(line.X2, line.Y2, layer);
    int x1 = line.X1;
    int y1 = line.Y1;
    int x2 = line.X2;
    int y2 = line.Y2;
    SDL_Color color = MppColorGet(line.Color);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderLine(renderer, x1, y1, x2, y2);
}

void MppRendererSubmit()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    for (int layer = 0; layer < MppRendererLayerCount; layer++)
    {
        Commands& commands = layers[layer];
        std::sort(commands.Sprites.begin(), commands.Sprites.end());
        for (Sprite& sprite : commands.Sprites)
        {
            DrawSprite(sprite, MppRendererLayer(layer));
        }
        for (Quad& quad : commands.Quads)
        {
            DrawQuad(quad, MppRendererLayer(layer));
        }
        for (Line& line : commands.Lines)
        {
            DrawLine(line, MppRendererLayer(layer));
        }
        commands.Sprites.clear();
        commands.Quads.clear();
        commands.Lines.clear();
    }
    SDL_RenderPresent(renderer);
    int min = 0;
    int max = MppWorldGetSize() - 1;
    tileX1 = std::clamp(worldX / MppTile::kSize - 1, min, max);
    tileY1 = std::clamp(worldY / MppTile::kSize - 1, min, max);
    tileX2 = std::clamp((worldX + kWidth) / MppTile::kSize + 1, min, max);
    tileY2 = std::clamp((worldY + kHeight) / MppTile::kSize + 1, min, max);
}

void MppRendererDraw(MppSprite sprite, int x, int y, bool flip, MppRendererLayer layer)
{
    layers[layer].Sprites.emplace_back(sprite, x, y, flip);
}

void MppRendererDrawRect(int color, int x, int y, int width, int height, MppRendererLayer layer)
{
    layers[layer].Quads.emplace_back(color, x, y, width, height);
}

void MppRendererDrawLine(int color, int x1, int y1, int x2, int y2, MppRendererLayer layer)
{
    layers[layer].Lines.emplace_back(color, x1, y1, x2, y2);
}

int MppRendererGetTileX1()
{
    return tileX1;
}

int MppRendererGetTileY1()
{
    return tileY1;
}

int MppRendererGetTileX2()
{
    return tileX2;
}

int MppRendererGetTileY2()
{
    return tileY2;
}
