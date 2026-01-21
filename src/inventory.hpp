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
    void Visit(SavepointVisitor& visitor);
    void Render() const override;
    bool Add(const MppItem& item);
    bool IsEmpty() const;
    void OnAction() override;
    void OnUp() override;
    void OnDown() override;
    void OnHeldUp() override;
    void OnHeldDown() override;
    void OnRender() override;

private:
    std::vector<MppItem> Items;
    std::array<int, MppInventorySlotCount> Slots;
};
