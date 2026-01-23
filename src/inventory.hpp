#pragma once

#include <savepoint/fwd.hpp>

#include <array>
#include <vector>

#include "input.hpp"
#include "item.hpp"
#include "menu.hpp"

enum MppInventorySlot
{
    MppInventorySlotHeld,
    MppInventorySlotHelmet,
    MppInventorySlotCuirass,
    MppInventorySlotLeggings,
    MppInventorySlotBoots,
    MppInventorySlotCount,
};

class MppInventory
    : public MppMenu
    , public MppInputHandler
{
public:
    MppInventory();
    void Visit(SavepointVisitor& visitor);
    void Render() const override;
    bool Add(const MppItem& item);
    void SetMaxItems(int max);
    bool IsEmpty() const;
    void OnAction() override;
    void OnUpArrow() override;
    void OnDownArrow() override;
    void OnHeldUp() override;
    void OnHeldDown() override;
    void OnRender() override;

private:
    std::vector<MppItem> Items;
    std::array<int, MppInventorySlotCount> Slots;
    int MaxItems;
    int Top;
    int Index;
};
