#pragma once

#include <memory>

#include "mob_inventory.hpp"

class MppHumanoidInventory : public MppMobInventory
{
public:
    enum SlotType
    {
        SlotTypeHelmet,
        SlotTypeCuirass,
        SlotTypeLeggings,
        SlotTypeBoots,
        SlotTypeHands,
        SlotTypeCount,
    };

    MppHumanoidInventory();
    void Action() override;
    const MppItem* GetHelmet() const;
    const MppItem* GetCuirass() const;
    const MppItem* GetLeggings() const;
    const MppItem* GetBoots() const;
    const MppItem* GetHands() const;
};