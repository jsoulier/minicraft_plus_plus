#include <SDL3/SDL.h>

#include <memory>

#include "humanoid_inventory.hpp"

MppHumanoidInventory::MppHumanoidInventory()
    : MppMobInventory(SlotTypeCount)
{
}

void MppHumanoidInventory::Action()
{
    const MppItem* item = GetItem();
    if (!item)
    {
        return;
    }
    if (item->GetFlag() & MppItemFlagHeld)
    {
        SetSlot(SlotTypeHands);
    }
    else if (item->GetFlag() & MppItemFlagArmor)
    {
        switch (item->GetType())
        {
        case MppItemTypeHelmet:
            SetSlot(SlotTypeHelmet);
            break;
        case MppItemTypeCuirass:
            SetSlot(SlotTypeCuirass);
            break;
        case MppItemTypeLeggings:
            SetSlot(SlotTypeLeggings);
            break;
        case MppItemTypeBoots:
            SetSlot(SlotTypeBoots);
            break;
        default:
            SDL_assert(false);
        }
    }
    else if (item->GetFlag() & MppItemFlagConsumable)
    {
        // TODO:
    }
}

const MppItem* MppHumanoidInventory::GetHelmet() const
{
    return GetItemFromSlot(SlotTypeHelmet);
}

const MppItem* MppHumanoidInventory::GetCuirass() const
{
    return GetItemFromSlot(SlotTypeCuirass);
}

const MppItem* MppHumanoidInventory::GetLeggings() const
{
    return GetItemFromSlot(SlotTypeLeggings);
}

const MppItem* MppHumanoidInventory::GetBoots() const
{
    return GetItemFromSlot(SlotTypeBoots);
}

const MppItem* MppHumanoidInventory::GetHands() const
{
    return GetItemFromSlot(SlotTypeHands);
}
