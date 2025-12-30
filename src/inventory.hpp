#pragma once

#include <savepoint/fwd.hpp>

#include <string_view>
#include <vector>

#include "item.hpp"
#include "menu_list.hpp"

class MppRenderer;

class MppInventory : public MppMenuList
{
public:
    void Draw(MppRenderer& renderer) override;
    void Visit(SavepointVisitor& visitor);
    void Add(const MppItem& item);
    void Remove(int index);
    bool Remove(MppItemID id);

protected:
    const MppItem* GetItem() const;
    const MppItem* GetItem(int index) const;
    void Draw(MppRenderer& renderer, int y, int index) override;
    std::string_view GetName() const override;
    int GetX() const override;
    int GetY() const override;
    int GetWidth() const override;
    int GetHeight() const override;
    int GetRowHeight() const override;
    virtual void RemoveInternal(int index) {}

private:
    std::vector<MppItem> Items;
};