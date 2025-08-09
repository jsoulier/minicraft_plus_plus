#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <nlohmann/json.hpp>

#include <cassert>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

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

template<typename T>
struct FontTextureKeyImpl
{
    FontTextureKeyImpl(const std::string_view& text, int colour, int size)
        : text{text}
        , colour{colour}
        , size{size} {}

    template<typename V>
    bool operator==(const V& other) const
    {
        return
            text == other.text &&
            colour == other.colour &&
            size == other.size;
    }

    T text;
    int colour;
    int size;
};

template<typename T>
struct ScreenTextureKeyImpl
{
    ScreenTextureKeyImpl(const std::string_view& text, int colour1, int colour2, int colour3, int colour4, int colour5)
        : text{text}
        , colour1{colour1}
        , colour2{colour2}
        , colour3{colour3}
        , colour4{colour4}
        , colour5{colour5} {}

    template<typename V>
    bool operator==(const V& other) const
    {
        return
            text == other.text &&
            colour1 == other.colour1 &&
            colour2 == other.colour2 &&
            colour3 == other.colour3 &&
            colour4 == other.colour4 &&
            colour5 == other.colour5;
    }

    std::string text;
    int colour1;
    int colour2;
    int colour3;
    int colour4;
    int colour5;
};

using FontTextureKey = FontTextureKeyImpl<std::string>;
using FontTextureKeyView = FontTextureKeyImpl<std::string_view>;
using ScreenTextureKey = ScreenTextureKeyImpl<std::string>;
using ScreenTextureKeyView = ScreenTextureKeyImpl<std::string_view>;

struct Hash
{
    using is_transparent = void;

    template<typename T>
    size_t operator()(const FontTextureKeyImpl<T>& key) const
    {
        size_t hash = 0;
        hash |= std::hash<std::string_view>{}(key.text);
        hash ^= std::hash<int>{}(key.colour);
        hash ^= std::hash<int>{}(key.size);
        return hash;
    }

    template<typename T>
    size_t operator()(const ScreenTextureKeyImpl<T>& key) const
    {
        size_t hash = 0;
        hash |= std::hash<std::string_view>{}(key.text);
        hash ^= std::hash<int>{}(key.colour1);
        hash ^= std::hash<int>{}(key.colour2);
        hash ^= std::hash<int>{}(key.colour3);
        hash ^= std::hash<int>{}(key.colour4);
        hash ^= std::hash<int>{}(key.colour5);
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
static std::unordered_map<uint64_t, SDL_Surface*> SpriteSurfaces;
static std::unordered_map<uint64_t, SDL_Texture*> SpriteTextures;
static std::unordered_map<int, TTF_Font*> Fonts;
static std::unordered_map<FontTextureKey, SDL_Texture*, Hash, std::equal_to<>> FontTextures;
static std::unordered_map<std::string, SDL_Surface*, Hash, std::equal_to<>> ScreenSurfaces;
static std::unordered_map<ScreenTextureKey, SDL_Texture*, Hash, std::equal_to<>> ScreenTextures;

static SDL_Color GetColour(int value)
{
    int red = (value / 100) % 10;
    int green = (value / 10) % 10;
    int blue = value % 10;
    assert(red >= 0 && red <= 9);
    assert(green >= 0 && green <= 9);
    assert(blue >= 0 && blue <= 9);
    SDL_Color colour;
    colour.r = red * 255 / 9;
    colour.g = green * 255 / 9;
    colour.b = blue * 255 / 9;
    colour.a = 255;
    return colour;
}

static uint64_t GetSpriteSurfaceKey(MppSprite sprite)
{
    return (sprite >> 50) & 0x3FFF;
}

static uint64_t GetSpriteTextureKey(MppSprite sprite)
{
    return sprite;
}

static SDL_Color GetSpriteColour1(MppSprite sprite)
{
    return GetColour((sprite >> 0) & 0x3FF);
}

static SDL_Color GetSpriteColour2(MppSprite sprite)
{
    return GetColour((sprite >> 10) & 0x3FF);
}

static SDL_Color GetSpriteColour3(MppSprite sprite)
{
    return GetColour((sprite >> 20) & 0x3FF);
}

static SDL_Color GetSpriteColour4(MppSprite sprite)
{
    return GetColour((sprite >> 30) & 0x3FF);
}

static SDL_Color GetSpriteColour5(MppSprite sprite)
{
    return GetColour((sprite >> 40) & 0x3FF);
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
    for (auto& [key, value] : ScreenSurfaces)
    {
        SDL_DestroySurface(value);
    }
    for (auto& [key, value] : ScreenTextures)
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
    uint64_t surfaceKey = GetSpriteSurfaceKey(sprite);
    uint64_t textureKey = GetSpriteTextureKey(sprite);
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
        SDL_Color colours[6];
        colours[0] = GetSpriteColour1(sprite);
        colours[1] = GetSpriteColour2(sprite);
        colours[2] = GetSpriteColour3(sprite);
        colours[3] = GetSpriteColour4(sprite);
        colours[4] = GetSpriteColour5(sprite);
        colours[5].a = 0;
        SDL_SetPaletteColors(Palette, colours, 0, 6);
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

void MppDraw(const std::string_view& text, float x, float y, int colour, int size)
{
    size *= kFontResolution;
    FontTextureKeyView textureKeyView{text, colour, size};
    auto textureIt = FontTextures.find(textureKeyView);
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
        SDL_Surface* surface = TTF_RenderText_Blended(fontIt->second, text.data(), text.size(), GetColour(colour));
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
        FontTextureKey textureKey{text, colour, size};
        textureIt = FontTextures.emplace(std::move(textureKey), texture).first;
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

void MppDraw(const std::string_view& name, int colour1, int colour2, int colour3, int colour4, int colour5)
{
    ScreenTextureKeyView textureKeyView{name, colour1, colour2, colour3, colour4, colour5};
    auto textureIt = ScreenTextures.find(textureKeyView);
    if (textureIt == ScreenTextures.end())
    {
        auto surfaceIt = ScreenSurfaces.find(name);
        if (surfaceIt == ScreenSurfaces.end())
        {
            SDL_Surface* surface = IMG_Load(name.data());
            if (!surface)
            {
                SDL_Log("Failed to load surface: %s, %s", name.data(), SDL_GetError());
                return;
            }
            surfaceIt = ScreenSurfaces.emplace(name, surface).first;
        }
        if (!surfaceIt->second)
        {
            return;
        }
        SDL_Color colours[6];
        colours[0] = GetColour(colour1);
        colours[1] = GetColour(colour2);
        colours[2] = GetColour(colour3);
        colours[3] = GetColour(colour4);
        colours[4] = GetColour(colour5);
        colours[5].a = 0;
        SDL_SetPaletteColors(Palette, colours, 0, 6);
        SDL_SetSurfacePalette(surfaceIt->second, Palette);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, surfaceIt->second);
        if (!texture)
        {
            SDL_Log("Failed to create texture: %s, %s", name.data(), SDL_GetError());
            return;
        }
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        ScreenTextureKey textureKey{name, colour1, colour2, colour3, colour4, colour5};
        textureIt = ScreenTextures.emplace(std::move(textureKey), texture).first;
    }
    if (!textureIt->second)
    {
        return;
    }
    SDL_RenderTexture(Renderer, textureIt->second, nullptr, nullptr);
}