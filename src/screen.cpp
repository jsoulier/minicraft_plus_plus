#include "color.hpp"
#include "renderer.hpp"
#include "screen.hpp"

void MppScreen::Draw(MppRenderer& renderer)
{
    renderer.DrawRect(GetBackgroundColor(), GetX(), GetY(), GetWidth(), GetHeight(), MppRenderer::LayerScreen);
}

void MppScreen::Draw(MppRenderer& renderer, const std::string_view& string, int color, int x, int y)
{

}

int MppScreen::GetBorderColor1() const
{
    return 0;
}

int MppScreen::GetBorderColor2() const
{
    return 0;
}

int MppScreen::GetBackgroundColor() const
{
    return 0;
}

int MppScreen::GetContentX() const
{
    return GetX();
}

int MppScreen::GetContentY() const
{
    return GetY();
}

int MppScreen::GetContentWidth() const
{
    return GetWidth();
}

int MppScreen::GetContentHeight() const
{
    return GetHeight();
}
