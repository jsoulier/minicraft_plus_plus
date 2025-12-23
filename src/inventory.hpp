#pragma once

#include <savepoint_fwd.hpp>

#include <string_view>
#include <vector>

#include "item.hpp"
#include "menu_list.hpp"

class MppRenderer;

class MppInventory : public MppMenuList
{
public:
    MppInventory();
    void Draw(MppRenderer& renderer);
    void Visit(SavepointVisitor& visitor);
    void Add(const MppItem& item);
    void Remove(int index);

protected:
    void Draw(MppRenderer& renderer, int y, int index) override;
    std::string_view GetName() const override;
    int GetX() const override;
    int GetY() const override;
    int GetWidth() const override;
    int GetHeight() const override;
    int GetRowHeight() const override;

private:
    std::vector<MppItem> Items;
};