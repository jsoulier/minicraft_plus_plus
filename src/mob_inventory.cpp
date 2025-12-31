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

int MppMobInventory::Slot::GetIndex() const
{
    return Index;
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
    OnSetSlot(slot, Slots[slot]);
}
    
void MppMobInventory::RemoveInternal(int index)
{
    for (int i = 0; i < Slots.size(); i++)
    {
        Slot& slot = Slots[i];
        if (slot.Index == index)
        {
            slot.Index = kInvalidIndex;
            OnSetSlot(i, slot);
        }
        else if (slot.Index > index)
        {
            slot.Index--;
            if (slot.Index < 0)
            {
                slot.Index = kInvalidIndex;
                OnSetSlot(i, slot);
            }
        }
    }
}
