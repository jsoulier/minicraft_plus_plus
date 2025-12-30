#pragma once

#include <savepoint/fwd.hpp>

#include <memory>
#include <string_view>
#include <vector>

#include "item.hpp"
#include "menu_list.hpp"

class MppRenderer;

class MppInventory : public MppMenuList
{
public:
    class Slot
    {
        friend class MppInventory;

    public:
        Slot();
        void Visit(SavepointVisitor& visitor);
        bool operator==(const Slot other) const;
        bool IsValid() const;

    public:
        int Index;
    };

    MppInventory(int slots);
    void Draw(MppRenderer& renderer) override;
    void Visit(SavepointVisitor& visitor);
    void Add(const MppItem& item);
    void Remove(int index);
    bool Remove(MppItemID id);

protected:
    const MppItem* GetItem() const;
    const MppItem* GetItem(int slot) const;
    void SetSlot(int slot);
    void Draw(MppRenderer& renderer, int y, int index) override;
    std::string_view GetName() const override;
    int GetX() const override;
    int GetY() const override;
    int GetWidth() const override;
    int GetHeight() const override;
    int GetRowHeight() const override;

private:
    std::vector<MppItem> Items;
    std::vector<Slot> Slots;
};