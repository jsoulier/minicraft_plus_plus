#pragma once

#include "menu.hpp"

class MppRenderer;

class MppMenuList : public MppMenu
{
public:
    MppMenuList();
    void Draw(MppRenderer& renderer) override;
    void Add();
    void Remove(int index);
    void Up();
    void Down();

protected:
    virtual void Draw(MppRenderer& renderer, int y, int index, bool selected) = 0;
    virtual int GetRowHeight() const = 0;

private:
    int Index;
    int First;
    int Max;
};