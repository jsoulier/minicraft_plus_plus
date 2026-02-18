#include <SDL3/SDL.h>

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <format>
#include <numeric>
#include <string>
#include <vector>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/sprite.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/world.hpp>

struct Sprite
{
    bool operator<(const Sprite& other) const
    {
        return Sprite < other.Sprite;
    }

    MppSprite Sprite;
    int X;
    int Y;
    MppRendererMod Mod;
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

struct Point
{
    int Color;
    int X;
    int Y;
};

struct Light
{
    int Color;
    int X;
    int Y;
    int Radius;
    int Strength;
};

static void DrawSprite(Sprite& sprite, MppRendererLayer layer);
static void DrawQuad(Quad& quad, MppRendererLayer layer);
static void DrawLine(Line& line, MppRendererLayer layer);
static void DrawPoint(Point& point, MppRendererLayer layer);

struct Commands
{
    void Draw(MppRendererLayer layer)
    {
        std::sort(Sprites.begin(), Sprites.end());
        for (Sprite& sprite : Sprites)
        {
            DrawSprite(sprite, MppRendererLayer(layer));
        }
        for (Quad& quad : Quads)
        {
            DrawQuad(quad, MppRendererLayer(layer));
        }
        for (Line& line : Lines)
        {
            DrawLine(line, MppRendererLayer(layer));
        }
        for (Point& point : Points)
        {
            DrawPoint(point, MppRendererLayer(layer));
        }
        Sprites.clear();
        Quads.clear();
        Lines.clear();
        Points.clear();
    }

    std::vector<Sprite> Sprites;
    std::vector<Quad> Quads;
    std::vector<Line> Lines;
    std::vector<Point> Points;
};

static constexpr int kWidth = 256;
static constexpr int kHeight = 144;
static const std::filesystem::path kBasePath = SDL_GetBasePath();
static const std::filesystem::path kSpritesheet = kBasePath / "spritesheet.png";

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Surface* spritesheet;
static SDL_Texture* lightTexture;
static std::unordered_map<size_t, SDL_Palette*> palettes;
static std::unordered_map<size_t, SDL_Surface*> surfaces;
static std::unordered_map<size_t, SDL_Texture*> textures;
static std::unordered_map<int, SDL_Texture*> lights;
static std::vector<Light> lightCommands;
static std::array<Commands, MppRendererLayerCount> layers;
static std::array<int, MppRendererLayerCount> layerIndices;
static int worldX;
static int worldY;
static int tileX1;
static int tileY1;
static int tileX2;
static int tileY2;

MppRendererLayerOverride::MppRendererLayerOverride(MppRendererLayer key, MppRendererLayer value)
    : Key{key}
    , Value{MppRendererLayer(layerIndices[key])}
{
    layerIndices[key] = value;
}

MppRendererLayerOverride::~MppRendererLayerOverride()
{
    layerIndices[Key] = Value;
}

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
    lightTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, kWidth, kHeight);
    if (!lightTexture)
    {
        MppLog("Failed to create light texture: %s", SDL_GetError());
        return false;
    }
    std::iota(layerIndices.begin(), layerIndices.end(), 0);
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
    SDL_DestroyTexture(lightTexture);
    SDL_DestroySurface(spritesheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    lightTexture = nullptr;
    spritesheet = nullptr;
    renderer = nullptr;
    window = nullptr;
}

void MppRendererMove(int x, int y, int size)
{
    // TODO: clamp to the world
    MppAssert(size > 0);
    worldX = x - kWidth / 2 + size / 2;
    worldY = y - kHeight / 2 + size / 2;
}

static void Move(int& x, int& y, MppRendererLayer layer)
{
    if (layer < MppRendererLayerMenu)
    {
        x -= worldX;
        y -= worldY;
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
    if (sprite.Mod == MppRendererModNone)
    {
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }
    else if (sprite.Mod == MppRendererModFlipHorizontally)
    {
        SDL_RenderTextureRotated(renderer, texture, nullptr, &rect, 180.0f, nullptr, SDL_FLIP_VERTICAL);
    }
    else if (sprite.Mod == MppRendererModFlipVertically)
    {
        SDL_RenderTextureRotated(renderer, texture, nullptr, &rect, 180.0f, nullptr, SDL_FLIP_HORIZONTAL);
    }
    else if (sprite.Mod == MppRendererModRotate90CW)
    {
        SDL_RenderTextureRotated(renderer, texture, nullptr, &rect, -90.0f, nullptr, SDL_FLIP_VERTICAL);
    }
    else if (sprite.Mod == MppRendererModRotate90CCW)
    {
        SDL_RenderTextureRotated(renderer, texture, nullptr, &rect, 90.0f, nullptr, SDL_FLIP_VERTICAL);
    }
    else
    {
        MppAssert(false);
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

static void DrawPoint(Point& point, MppRendererLayer layer)
{
    Move(point.X, point.Y, layer);
    int x = point.X;
    int y = point.Y;
    SDL_Color color = MppColorGet(point.Color);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderPoint(renderer, x, y);
}

static void DrawLight(Light light)
{
    int size = light.Radius * 2 + 1;
    auto lightIt = lights.find(light.Radius);
    if (lightIt == lights.end())
    {
        SDL_Surface* surface = SDL_CreateSurface(size, size, SDL_PIXELFORMAT_RGBA32);
        if (!surface)
        {
            MppLog("Failed to create surface: %s", SDL_GetError());
            return;
        }
        SDL_ClearSurface(surface, 0.0f, 0.0f, 0.0f, 0.0f);
        SDL_LockSurface(surface);
        for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++)
        {
            float dx = light.Radius - x;
            float dy = light.Radius - y;
            float distance = std::sqrt(dx * dx + dy * dy);
            float radius = light.Radius;
            if (distance > radius)
            {
                continue;
            }
            float ditherRadius = radius * 0.5f;
            if (distance <= ditherRadius)
            {
                SDL_WriteSurfacePixel(surface, x, y, 255, 255, 255, 255);
                continue;
            }
            float ratio = (distance - ditherRadius) / (radius - ditherRadius);
            ratio = std::clamp(ratio, 0.0f, 1.0f);
            float density = 1.0f - ratio;
            density *= density;
            static constexpr int kBayer[4][4] =
            {
                { 0, 8, 2,10},
                {12, 4,14, 6},
                { 3,11, 1, 9},
                {15, 7,13, 5}
            };
            float threshold = kBayer[y % 4][x % 4] / 16.0f;
            if (density > threshold)
            {
                SDL_WriteSurfacePixel(surface, x, y, 255, 255, 255, 255);
            }
        }
        SDL_UnlockSurface(surface);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);
        if (!texture)
        {
            MppLog("Failed to create texture: %s", SDL_GetError());
            return;
        }
        lightIt = lights.emplace(light.Radius, texture).first;
    }
    Move(light.X, light.Y, MppRendererLayerEntity);
    SDL_Texture* texture = lightIt->second;
    SDL_Color color = MppColorGet(light.Color);
    SDL_FRect rect;
    rect.x = light.X - light.Radius;
    rect.y = light.Y - light.Radius;
    rect.w = size;
    rect.h = size;
    MppAssert(light.Strength >= 0 && light.Strength <= 5);
    SDL_SetTextureAlphaMod(texture, light.Strength * 255 / 5);
    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_RenderTexture(renderer, texture, nullptr, &rect);
}

void MppRendererSubmit(int inLightColor)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_RenderClear(renderer);
    for (int layer = 0; layer < MppRendererLayerLightEnd; layer++)
    {
        layers[layer].Draw(MppRendererLayer(layer));
    }
    SDL_Color lightColor = MppColorGet(inLightColor);
    SDL_SetRenderTarget(renderer, lightTexture);
    SDL_SetRenderDrawColor(renderer, lightColor.r, lightColor.g, lightColor.b, lightColor.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    for (Light& light : lightCommands)
    {
        DrawLight(light);
    }
    SDL_SetTextureBlendMode(lightTexture, SDL_BLENDMODE_MOD);
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderTexture(renderer, lightTexture, nullptr, nullptr);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    for (int layer = MppRendererLayerLightEnd; layer < MppRendererLayerCount; layer++)
    {
        layers[layer].Draw(MppRendererLayer(layer));
    }
    lightCommands.clear();
    SDL_RenderPresent(renderer);
    int min = 0;
    int max = MppWorldGetSize() - 1;
    tileX1 = std::clamp(MppWorldGetTileIndex(worldX) - 1, min, max);
    tileY1 = std::clamp(MppWorldGetTileIndex(worldY) - 1, min, max);
    tileX2 = std::clamp(MppWorldGetTileIndex(worldX + kWidth) + 1, min, max);
    tileY2 = std::clamp(MppWorldGetTileIndex(worldY + kHeight) + 1, min, max);
}

void MppRendererDraw(MppSprite sprite, int x, int y, MppRendererMod mod, MppRendererLayer layer)
{
    layers[layerIndices[layer]].Sprites.emplace_back(sprite, x, y, mod);
}

void MppRendererDrawRect(int color, int x, int y, int width, int height, MppRendererLayer layer)
{
    layers[layerIndices[layer]].Quads.emplace_back(color, x, y, width, height);
}

void MppRendererDrawLine(int color, int x1, int y1, int x2, int y2, MppRendererLayer layer)
{
    layers[layerIndices[layer]].Lines.emplace_back(color, x1, y1, x2, y2);
}

void MppRendererDrawPoint(int color, int x, int y, MppRendererLayer layer)
{
    layers[layerIndices[layer]].Points.emplace_back(color, x, y);
}

void MppRendererDrawLight(int color, int x, int y, int radius, int strength)
{
    lightCommands.emplace_back(color, x, y, radius, strength);
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
