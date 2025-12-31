#pragma once

#include <functional>
#include <memory>

#include "mob_inventory.hpp"

using MppHumanoidInventoryHeldCallback = std::function<void(int index, const MppItem* item)>;

class MppHumanoidInventory : public MppMobInventory
{
public:
    enum SlotType
    {
        SlotTypeHelmet,
        SlotTypeCuirass,
        SlotTypeLeggings,
        SlotTypeBoots,
        SlotTypeHeld,
        SlotTypeCount,
    };

    MppHumanoidInventory();
    void Action() override;
    void SetHeldCallback(const MppHumanoidInventoryHeldCallback& callback);
    const MppItem* GetHelmet() const;
    const MppItem* GetCuirass() const;
    const MppItem* GetLeggings() const;
    const MppItem* GetBoots() const;
    const MppItem* GetHeld() const;

protected:
    void OnSetSlot(int position, const Slot& slot) override;

private:
    MppHumanoidInventoryHeldCallback HeldCallback;
};