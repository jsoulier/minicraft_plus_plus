#include "color.hpp"
#include "menu.hpp"
#include "renderer.hpp"

MppMenu::MppMenu()
    : FgColor{kMppColorMenuFg}
    , BgColor{kMppColorMenuBg}
    , X1{8}
    , Y1{8}
    , X2{248}
    , Y2{136}
{
}

void MppMenu::Render()
{
    MppRendererDrawRect(FgColor, X1, Y1, GetWidth(), GetHeight(), MppRendererLayerMenu);
}

void MppMenu::SetFgColor(int color)
{
    FgColor = color;
}

void MppMenu::SetBgColor(int color)
{
    BgColor = color;
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
