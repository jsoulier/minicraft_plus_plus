#pragma once

#include <memory>

#include "inventory.hpp"

class MppHumanoidInventory : public MppInventory
{
public:
    enum SlotType
    {
        SlotTypeHelmet,
        SlotTypeChestplate,
        SlotTypeLeggings,
        SlotTypeBoots,
        SlotTypeHands,
        SlotTypeCount,
    };

    MppHumanoidInventory();
    void Action() override;
    const MppItem* GetHelmet() const;
    const MppItem* GetChestplate() const;
    const MppItem* GetLeggings() const;
    const MppItem* GetBoots() const;
    const MppItem* GetHands() const;
};