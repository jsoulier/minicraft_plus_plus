#pragma once

#include <string_view>

#include <minicraft++/input.hpp>

enum MppMenuAlignment
{
    MppMenuAlignmentCenter,
    MppMenuAlignmentLeft,
    MppMenuAlignmentRight,
};

class MppMenu
{
public:
    MppMenu();
    void Render() const;
    void SetX1(int x1);
    void SetY1(int y1);
    void SetX2(int x2);
    void SetY2(int y2);

protected:
    void Render(const std::string_view& string, int color, int x, int y, MppMenuAlignment alignment) const;
    int GetWidth() const;
    int GetHeight() const;

protected:
    int X1;
    int Y1;
    int X2;
    int Y2;
};
