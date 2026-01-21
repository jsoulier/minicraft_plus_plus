#pragma once

#include "input.hpp"

class MppMenu
{
public:
    MppMenu();
    virtual void Render() const;
    void SetFgColor(int color);
    void SetBgColor(int color);
    void SetX1(int x1);
    void SetY1(int y1);
    void SetX2(int x2);
    void SetY2(int y2);

private:
    int GetWidth() const;
    int GetHeight() const;

protected:
    int FgColor;
    int BgColor;
    int X1;
    int Y1;
    int X2;
    int Y2;
};
