#include <SDL3/SDL.h>

#include <memory>

#include "humanoid_inventory.hpp"

MppHumanoidInventory::MppHumanoidInventory()
    : MppInventory(SlotTypeCount)
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
        case MppItemTypeChestplate:
            SetSlot(SlotTypeChestplate);
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
    return GetItem(SlotTypeHelmet);
}

const MppItem* MppHumanoidInventory::GetChestplate() const
{
    return GetItem(SlotTypeChestplate);
}

const MppItem* MppHumanoidInventory::GetLeggings() const
{
    return GetItem(SlotTypeLeggings);
}

const MppItem* MppHumanoidInventory::GetBoots() const
{
    return GetItem(SlotTypeBoots);
}

const MppItem* MppHumanoidInventory::GetHands() const
{
    return GetItem(SlotTypeHands);
}
