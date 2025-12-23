#pragma once

#include <string_view>

class MppRenderer;

class MppMenu
{
public:
    void Draw(MppRenderer& renderer);
    void Draw(MppRenderer& renderer, const std::string_view& string, int color, int x, int y);

protected:
    virtual std::string_view GetName() const = 0;
    virtual int GetBorderColor1() const;
    virtual int GetBorderColor2() const;
    virtual int GetBackgroundColor() const;
    virtual int GetX() const = 0;
    virtual int GetY() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
    int GetContentX() const;
    int GetContentY() const;
    int GetContentWidth() const;
    int GetContentHeight() const;
};