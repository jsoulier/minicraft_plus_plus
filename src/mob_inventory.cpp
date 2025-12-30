#include "color.hpp"
#include "item.hpp"
#include "mob_inventory.hpp"
#include "renderer.hpp"

static constexpr int kInvalidIndex = -1;

MppMobInventory::Slot::Slot()
    : Index{kInvalidIndex}
{
}

void MppMobInventory::Slot::Visit(SavepointVisitor& visitor)
{
    visitor(Index);
}

bool MppMobInventory::Slot::operator==(const Slot other) const
{
    return Index == other.Index;
}

bool MppMobInventory::Slot::IsValid() const
{
    return Index != kInvalidIndex;
}

MppMobInventory::MppMobInventory(int slots)
    : MppInventory()
    , Slots(slots)
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
}

void MppMobInventory::Visit(SavepointVisitor& visitor)
{
    MppInventory::Visit(visitor);
    if (visitor.IsReading())
    {
        Slots.clear();
    }
    visitor(Slots);
}

void MppMobInventory::Draw(MppRenderer& renderer, int y, int index)
{
    MppInventory::Draw(renderer, y, index);
    // TODO: remove nested loop
    for (const Slot& slot : Slots)
    {
        if (slot.Index == index)
        {
            MppMenu::Draw(renderer, "E", kMppColorText, GetContentX() + 8, y);
        }
    }
}

const MppItem* MppMobInventory::GetItemFromSlot(int slot) const
{
    if (Slots[slot].IsValid())
    {
        return MppInventory::GetItem(Slots[slot].Index);
    }
    else
    {
        return nullptr;
    }
}

void MppMobInventory::SetSlot(int slot)
{
    if (Slots[slot].Index == Index)
    {
        Slots[slot].Index = kInvalidIndex;
    }
    else
    {
        Slots[slot].Index = Index;
    }
}
    
void MppMobInventory::RemoveInternal(int index)
{
    for (Slot& slot : Slots)
    {
        if (slot.Index == index)
        {
            slot.Index = kInvalidIndex;
        }
        else if (slot.Index < index)
        {
            slot.Index--;
            if (slot.Index < 0)
            {
                slot.Index = kInvalidIndex;
            }
        }
    }
}
