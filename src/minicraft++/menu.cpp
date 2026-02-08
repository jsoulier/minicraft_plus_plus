#include <cctype>
#include <string_view>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/menu.hpp>
#include <minicraft++/renderer.hpp>

static constexpr int kBackgroundColor = 5;

MppMenu::MppMenu()
    : X1{8}
    , Y1{8}
    , X2{248}
    , Y2{136}
{
}

void MppMenu::Render() const
{
    MppRendererDrawRect(kBackgroundColor, X1, Y1, GetWidth(), GetHeight(), MppRendererLayerMenu);
}

void MppMenu::Render(const std::string_view& string, int color, int x, int y, MppMenuAlignment alignment) const
{
    switch (alignment)
    {
    case MppMenuAlignmentCenter:
        x -= string.size() * 8 / 2;
        break;
    case MppMenuAlignmentLeft:
        x += 0;
        break;
    case MppMenuAlignmentRight:
        x -= string.size() * 8;
        break;
    }
    for (char character : string)
    {
        MppSprite sprite(color, std::toupper(character));
        MppRendererDraw(sprite, x, y, MppRendererFlipNone, MppRendererLayerMenuContent);
        x += 8;
    }
}

void MppMenu::SetX1(int x1)
{
    X1 = x1;
}

void MppMenu::SetY1(int y1)
{
    Y1 = y1;
}

void MppMenu::SetX2(int x2)
{
    X2 = x2;
}

void MppMenu::SetY2(int y2)
{
    Y2 = y2;
}

int MppMenu::GetWidth() const
{
    return X2 - X1;
}

int MppMenu::GetHeight() const
{
    return Y2 - Y1;
}
