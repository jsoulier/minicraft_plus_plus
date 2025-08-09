#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <nlohmann/json.hpp>

#include <cassert>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

#include "window.hpp"

static constexpr const char* kTitle = "Minicraft++";
static constexpr int kWidth = 256;
static constexpr int kHeight = 144;
static constexpr const char* kSpritesheet = "spritesheet.png";
static constexpr const char* kFont = "raster_forge.ttf";
static constexpr int kSpriteAlignment = 16;
static constexpr int kSpriteSizeToSize[] = {2, 4, 8, 16};
static constexpr int kFontResolution = 4;
static_assert(SDL_arraysize(kSpriteSizeToSize) == MppSpriteSizeCount);

struct FontTextureKey
{
    FontTextureKey(const std::string_view& text, MppColor color, int size)
        : text{text}
        , color{color}
        , size{size} {}

    bool operator==(const FontTextureKey& other) const
    {
        return text == other.text && color == other.color && size == other.size;
    }

    std::string text;
    MppColor color;
    int size;
};

struct Hash
{
    using is_transparent = void;

    size_t operator()(const FontTextureKey& key) const
    {
        size_t hash = 0;
        hash |= std::hash<std::string>{}(key.text);
        hash ^= std::hash<MppColor>{}(key.color);
        hash ^= std::hash<int>{}(key.size);
        return hash;
    }

    size_t operator()(const std::string_view& key) const
    {
        return std::hash<std::string_view>{}(key);
    }

    size_t operator()(const std::string& key) const
    {
        return std::hash<std::string>{}(key);
    }
};

static SDL_Window* Window;
static SDL_Renderer* Renderer;
static SDL_GPUDevice* Device;
static TTF_TextEngine* TextEngine;
static SDL_Surface* Spritesheet;
static SDL_Palette* Palette;
static std::unordered_map<MppSprite, SDL_Surface*> SpriteSurfaces;
static std::unordered_map<MppSprite, SDL_Texture*> SpriteTextures;
static std::unordered_map<int, TTF_Font*> Fonts;
static std::unordered_map<FontTextureKey, SDL_Texture*, Hash> FontTextures;

static SDL_Color GetColor(MppColor value)
{
    uint8_t red = (value / 100) % 10;
    uint8_t green = (value / 10) % 10;
    uint8_t blue = value % 10;
    assert(red >= 0 && red <= 9);
    assert(green >= 0 && green <= 9);
    assert(blue >= 0 && blue <= 9);
    SDL_Color color;
    color.r = red * 255 / 9;
    color.g = green * 255 / 9;
    color.b = blue * 255 / 9;
    color.a = 255;
    return color;
}

static MppSprite GetSpriteSurfaceKey(MppSprite sprite)
{
    return (sprite >> 50) & 0x3FFF;
}

static MppSprite GetSpriteTextureKey(MppSprite sprite)
{
    return sprite;
}

static SDL_Color GetSpriteColor1(MppSprite sprite)
{
    return GetColor((sprite >> 0) & 0x3FF);
}

static SDL_Color GetSpriteColor2(MppSprite sprite)
{
    return GetColor((sprite >> 10) & 0x3FF);
}

static SDL_Color GetSpriteColor3(MppSprite sprite)
{
    return GetColor((sprite >> 20) & 0x3FF);
}

static SDL_Color GetSpriteColor4(MppSprite sprite)
{
    return GetColor((sprite >> 30) & 0x3FF);
}

static SDL_Color GetSpriteColor5(MppSprite sprite)
{
    return GetColor((sprite >> 40) & 0x3FF);
}

static int GetSpriteX(MppSprite sprite)
{
    return ((sprite >> 50) & 0x3F) * kSpriteAlignment;
}

static int GetSpriteY(MppSprite sprite)
{
    return ((sprite >> 56) & 0x3F) * kSpriteAlignment;
}

static int GetSpriteSize(MppSprite sprite)
{
    return kSpriteSizeToSize[(sprite >> 62) & 0x3];
}

bool MppCreateWindow()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }
    if (!TTF_Init())
    {
        SDL_Log("Failed to initialize SDL ttf: %s", SDL_GetError());
        return false;
    }
    Window = SDL_CreateWindow(kTitle, kWidth * 4, kHeight * 4, SDL_WINDOW_HIDDEN);
    if (!Window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
#if defined(SDL_PLATFORM_WIN32)
    Renderer = SDL_CreateGPURenderer(Window, SDL_GPU_SHADERFORMAT_DXIL, &Device);
#elif defined(SDL_PLATFORM_APPLE)
    Renderer = SDL_CreateGPURenderer(Window, SDL_GPU_SHADERFORMAT_MSL, &Device);
#elif defined(SDL_PLATFORM_LINUX)
    Renderer = SDL_CreateGPURenderer(Window, SDL_GPU_SHADERFORMAT_SPIRV, &Device);
#endif
    if (!Renderer)
    {
        SDL_Log("Failed to create device: %s", SDL_GetError());
        return false;
    }
    SDL_ShowWindow(Window);
    SDL_SetWindowResizable(Window, true);
    SDL_FlashWindow(Window, SDL_FLASH_BRIEFLY);
    SDL_SetRenderLogicalPresentation(Renderer, kWidth, kHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderVSync(Renderer, true);
    SDL_SetDefaultTextureScaleMode(Renderer, SDL_SCALEMODE_NEAREST);
    TextEngine = TTF_CreateGPUTextEngine(Device);
    if (!TextEngine)
    {
        SDL_Log("Failed to create text engine: %s", SDL_GetError());
        return false;
    }
    Spritesheet = IMG_Load(kSpritesheet);
    if (!Spritesheet)
    {
        SDL_Log("Failed to load spritesheet: %s", SDL_GetError());
        return false;
    }
    Palette = SDL_CreatePalette(6);
    if (!Palette)
    {
        SDL_Log("Failed to create palette: %s", SDL_GetError());
        return false;
    }
    return true;
}

void MppDestroyWindow()
{
    for (auto& [key, value] : SpriteSurfaces)
    {
        SDL_DestroySurface(value);
    }
    for (auto& [key, value] : SpriteTextures)
    {
        SDL_DestroyTexture(value);
    }
    for (auto& [key, value] : Fonts)
    {
        TTF_CloseFont(value);
    }
    for (auto& [key, value] : FontTextures)
    {
        SDL_DestroyTexture(value);
    }
    SDL_DestroyPalette(Palette);
    SDL_DestroySurface(Spritesheet);
    TTF_DestroyGPUTextEngine(TextEngine);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    TTF_Quit();
    SDL_Quit();
}

void MppClearWindow()
{
    SDL_RenderClear(Renderer);
}

void MppPresentWindow()
{
    SDL_RenderPresent(Renderer);
}

void MppDraw(MppSprite sprite, float x, float y)
{
    MppSprite surfaceKey = GetSpriteSurfaceKey(sprite);
    MppSprite textureKey = GetSpriteTextureKey(sprite);
    auto textureIt = SpriteTextures.find(textureKey);
    if (textureIt == SpriteTextures.end())
    {
        auto surfaceIt = SpriteSurfaces.find(surfaceKey);
        if (surfaceIt == SpriteSurfaces.end())
        {
            int size = GetSpriteSize(sprite);
            SDL_Rect rect;
            rect.x = GetSpriteX(sprite);
            rect.y = GetSpriteY(sprite);
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
        SDL_Color colors[6];
        colors[0] = GetSpriteColor1(sprite);
        colors[1] = GetSpriteColor2(sprite);
        colors[2] = GetSpriteColor3(sprite);
        colors[3] = GetSpriteColor4(sprite);
        colors[4] = GetSpriteColor5(sprite);
        colors[5].a = 0;
        SDL_SetPaletteColors(Palette, colors, 0, 6);
        SDL_SetSurfacePalette(surfaceIt->second, Palette);
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
    int size = GetSpriteSize(sprite);
    SDL_FRect rect;
    rect.x = x - size / 2;
    rect.y = y - size / 2;
    rect.w = size;
    rect.h = size;
    SDL_RenderTexture(Renderer, textureIt->second, nullptr, &rect);
}

void MppDraw(const std::string_view& text, float x, float y, MppColor color, int size)
{
    size *= kFontResolution;
    FontTextureKey textureKey{text, color, size};
    auto textureIt = FontTextures.find(textureKey);
    if (textureIt == FontTextures.end())
    {
        auto fontIt = Fonts.find(size);
        if (fontIt == Fonts.end())
        {
            TTF_Font* font = TTF_OpenFont(kFont, size);
            if (!font)
            {
                SDL_Log("Failed to open font: %s", SDL_GetError());
                return;
            }
            fontIt = Fonts.emplace(size, font).first;
        }
        if (!fontIt->second)
        {
            return;
        }
        SDL_Surface* surface = TTF_RenderText_Blended(fontIt->second, text.data(), text.size(), GetColor(color));
        if (!surface)
        {
            SDL_Log("Failed to create blended text surface: %s", SDL_GetError());
            return;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, surface);
        SDL_DestroySurface(surface);
        if (!texture)
        {
            SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
            return;
        }
        textureIt = FontTextures.emplace(textureKey, texture).first;
    }
    if (!textureIt->second)
    {
        return;
    }
    SDL_FRect rect;
    rect.w = (textureIt->second->w) / kFontResolution;
    rect.h = (textureIt->second->h) / kFontResolution;
    rect.x = x - rect.w / 2;
    rect.y = y - rect.h / 2;
    SDL_RenderTexture(Renderer, textureIt->second, nullptr, &rect);
}