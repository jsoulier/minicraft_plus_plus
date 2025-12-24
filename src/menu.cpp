#include <string_view>

#include "color.hpp"
#include "console.hpp"
#include "menu.hpp"
#include "renderer.hpp"
#include "sprite.hpp"

static constexpr int kTopOffsetY = 4;
static constexpr int kContentOffsetX = 2;
static constexpr int kTopContentOffsetY = 8;
static constexpr int kNumBorders = 2;

void MppMenu::Draw(MppRenderer& renderer)
{
    if (MppConsoleDebugScreens)
    {
        renderer.DrawRect(
            kMppColorRed,
            GetX() - 1,
            GetY() - 1,
            GetWidth() + 2,
            GetHeight() + 2,
            MppRenderer::LayerScreenDebug);
    }

    Draw(renderer, GetName(), kMppColorWhite, GetX() + GetWidth() / 2, GetY());

    // Top
    renderer.DrawLine(
        GetBorderColor1(),
        GetX() + 0,
        GetY() + kTopOffsetY,
        GetX() + GetWidth() - 1,
        GetY() + kTopOffsetY,
        MppRenderer::LayerScreen);
    renderer.DrawLine(
        GetBorderColor2(),
        GetX() + 1,
        GetY() + kTopOffsetY + 1,
        GetX() + GetWidth() - 2,
        GetY() + kTopOffsetY + 1,
        MppRenderer::LayerScreen);

    // Bottom
    renderer.DrawLine(
        GetBorderColor1(),
        GetX() + 0,
        GetY() + GetHeight() - 1,
        GetX() + GetWidth() - 1,
        GetY() + GetHeight() - 1,
        MppRenderer::LayerScreen);
    renderer.DrawLine(
        GetBorderColor2(),
        GetX() + 1,
        GetY() + GetHeight() - 2,
        GetX() + GetWidth() - 2,
        GetY() + GetHeight() - 2,
        MppRenderer::LayerScreen);

    // Left
    renderer.DrawLine(
        GetBorderColor1(),
        GetX() + 0,
        GetY() + kTopOffsetY,
        GetX() + 0,
        GetY() + GetHeight() - 1,
        MppRenderer::LayerScreen);
    renderer.DrawLine(
        GetBorderColor2(),
        GetX() + 1,
        GetY() + kTopOffsetY + 1,
        GetX() + 1,
        GetY() + GetHeight() - 2,
        MppRenderer::LayerScreen);

    // Right
    renderer.DrawLine(
        GetBorderColor1(),
        GetX() + GetWidth() - 1,
        GetY() + kTopOffsetY,
        GetX() + GetWidth() - 1,
        GetY() + GetHeight() - 1,
        MppRenderer::LayerScreen);
    renderer.DrawLine(
        GetBorderColor2(),
        GetX() + GetWidth() - 2,
        GetY() + kTopOffsetY + 1,
        GetX() + GetWidth() - 2,
        GetY() + GetHeight() - 2,
        MppRenderer::LayerScreen);

    // Background
    renderer.DrawRect(
        GetBackgroundColor(),
        GetX() + 2,
        GetY() + kTopOffsetY + kNumBorders,
        GetWidth() - kNumBorders * 2,
        GetHeight() - kTopOffsetY - kNumBorders * 2,
        MppRenderer::LayerScreen);
}

void MppMenu::Draw(MppRenderer& renderer, const std::string_view& string, int color, int x, int y)
{
    if (!string.size())
    {
        SDL_Log("Tried drawing an empty string");
        return;
    }
    x -= string.size() * 8 / 2;
    for (char character : string)
    {
        MppSprite sprite(character, color);
        renderer.Draw(sprite, x, y, false, MppRenderer::LayerScreenContent);
        x += 8;
    }
}

int MppMenu::GetBorderColor1() const
{
    return kMppColorBlack;
}

int MppMenu::GetBorderColor2() const
{
    return kMppColorLightGray;
}

int MppMenu::GetBackgroundColor() const
{
    return kMppColorBlue;
}

int MppMenu::GetContentX() const
{
    return GetX() + kContentOffsetX;
}

int MppMenu::GetContentY() const
{
    return GetY() + kTopContentOffsetY;
}

int MppMenu::GetContentWidth() const
{
    return GetWidth() - kContentOffsetX - kContentOffsetX;
}

int MppMenu::GetContentHeight() const
{
    return GetHeight() - kTopContentOffsetY - kTopOffsetY;
}
