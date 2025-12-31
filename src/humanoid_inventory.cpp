#include <SDL3/SDL.h>

#include <memory>

#include "humanoid_inventory.hpp"

MppHumanoidInventory::MppHumanoidInventory()
    : MppMobInventory(SlotTypeCount)
{
    // TODO: remove
    Add(MppItem{MppItemIDIronHelmet});
    Add(MppItem{MppItemIDIronHelmet});
    Add(MppItem{MppItemIDIronHelmet});
    Add(MppItem{MppItemIDApple});
    Add(MppItem{MppItemIDApple});
    Add(MppItem{MppItemIDIronHelmet});
    Add(MppItem{MppItemIDApple});
    Add(MppItem{MppItemIDIronCuirass});
    Add(MppItem{MppItemIDIronCuirass});
    Add(MppItem{MppItemIDIronCuirass});
    Add(MppItem{MppItemIDIronCuirass});
    Add(MppItem{MppItemIDIronLeggings});
    Add(MppItem{MppItemIDIronLeggings});
    Add(MppItem{MppItemIDIronLeggings});
    Add(MppItem{MppItemIDIronLeggings});
    Add(MppItem{MppItemIDIronBoots});
    Add(MppItem{MppItemIDIronBoots});
    Add(MppItem{MppItemIDIronBoots});
    Add(MppItem{MppItemIDIronBoots});
    Add(MppItem{MppItemIDWood});
    Add(MppItem{MppItemIDWood});
    Add(MppItem{MppItemIDApple});
    Add(MppItem{MppItemIDWorkbench});
    Add(MppItem{MppItemIDFurnace});
    Add(MppItem{MppItemIDWorkbench});
    Add(MppItem{MppItemIDFurnace});
    Add(MppItem{MppItemIDWorkbench});
    Add(MppItem{MppItemIDFurnace});
    Add(MppItem{MppItemIDWood});
    Add(MppItem{MppItemIDWood});
    Add(MppItem{MppItemIDWood});
    Add(MppItem{MppItemIDWood});
    Add(MppItem{MppItemIDWood});
    Add(MppItem{MppItemIDWood});
    Add(MppItem{MppItemIDWood});
    Add(MppItem{MppItemIDWood});
    Add(MppItem{MppItemIDWood});
    Add(MppItem{MppItemIDWood});
    Add(MppItem{MppItemIDStone});
    Add(MppItem{MppItemIDStone});
    Add(MppItem{MppItemIDStone});
    Add(MppItem{MppItemIDStone});
    Add(MppItem{MppItemIDStone});
    Add(MppItem{MppItemIDStone});
    Add(MppItem{MppItemIDStone});
    Add(MppItem{MppItemIDStone});
    Add(MppItem{MppItemIDStone});
    Add(MppItem{MppItemIDStone});
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
        SetSlot(SlotTypeHeld);
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

void MppHumanoidInventory::SetHeldCallback(const MppHumanoidInventoryHeldCallback& callback)
{
    HeldCallback = callback;
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

const MppItem* MppHumanoidInventory::GetHeld() const
{
    return GetItemFromSlot(SlotTypeHeld);
}

void MppHumanoidInventory::OnSetSlot(int position, const Slot& slot)
{
    if (position != SlotTypeHeld)
    {
        return;
    }
    if (HeldCallback)
    {
        HeldCallback(slot.GetIndex(), GetItem(slot.GetIndex()));
    }
}
