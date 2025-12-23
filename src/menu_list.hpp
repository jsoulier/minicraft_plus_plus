#pragma once

#include "menu.hpp"

class MppRenderer;

class MppMenuList : public MppMenu
{
public:
    MppMenuList();
    void Draw(MppRenderer& renderer);
    void Add();
    void Remove(int index);
    void Up();
    void Down();

protected:
    virtual void Draw(MppRenderer& renderer, int y, int index) = 0;
    virtual int GetRowHeight() const = 0;

private:
    int Index;
    int First;
    int Max;
};