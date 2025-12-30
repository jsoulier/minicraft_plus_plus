#pragma once

#include <savepoint/fwd.hpp>

#include "menu.hpp"

class MppRenderer;

class MppMenuList : public MppMenu
{
public:
    MppMenuList();
    void Visit(SavepointVisitor& visitor);
    void Draw(MppRenderer& renderer) override;
    void Add();
    void Remove(int index);
    void Up();
    void Down();
    virtual void Action() = 0;

protected:
    virtual void Draw(MppRenderer& renderer, int y, int index) = 0;
    virtual int GetRowHeight() const = 0;

    int Index;

private:
    int First;
    int Max;
};