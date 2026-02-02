#include <savepoint/savepoint.hpp>

#include <limits>
#include <string>

#include "assert.hpp"
#include "color.hpp"
#include "inventory.hpp"
#include "item.hpp"
#include "log.hpp"
#include "menu.hpp"
#include "renderer.hpp"
#include "util.hpp"

static constexpr int kInvalidSlot = std::numeric_limits<int>::max();

MppInventory::MppInventory()
    : MaxItems{0}
    , Focused{false}
{
    Clear();
}

void MppInventory::Visit(SavepointVisitor& visitor)
{
    visitor(Items);
    visitor(Slots);
    if (visitor.IsReading() && Items.size() > MaxItems)
    {
        MppLog("Read more then the max number of items");
    }
}

void MppInventory::Render() const
{
    MppMenu::Render();
    int maxRows = std::min(GetHeight() / 8, int(Items.size()) - Top);
    for (int row = 0; row < maxRows; row++)
    {
        int y = Y1 + row * 8;
        const MppItem& item = Items[Top + row];
        int count = item.GetCount();
        item.Render(X1 + 8, y, MppRendererLayerMenuContent);
        if (count > 1)
        {
            int offset = MppGetNumberOfDigits(count) * 8;
            MppMenu::Render(std::to_string(count), kMppColorMenuUnlocked, X1 + 16, y, MppMenuAlignmentLeft);
            MppMenu::Render(item.GetName(), kMppColorMenuUnlocked, X1 + 24 + offset, y, MppMenuAlignmentLeft);
        }
        else if (count == 1)
        {
            MppMenu::Render(item.GetName(), kMppColorMenuUnlocked, X1 + 16, y, MppMenuAlignmentLeft);
        }
        else if (count == 0)
        {
            MppMenu::Render(item.GetName(), kMppColorMenuLocked, X1 + 16, y, MppMenuAlignmentLeft);
        }
        else
        {
            MppAssert(false);
        }
    }
    if (Focused)
    {
        MppMenu::Render(">", kMppColorMenuUnlocked, X1, Y1 + (Index - Top) * 8, MppMenuAlignmentLeft);
    }
    for (int slot : Slots)
    {
        if (Focused && slot == Index)
        {
            continue;
        }
        if (slot < Top || slot > Top + maxRows)
        {
            continue;
        }
        MppMenu::Render("E", 333, X1, Y1 + (slot - Top) * 8, MppMenuAlignmentLeft);
    }
}

bool MppInventory::Add(const MppItem& item)
{
    if (item.GetType() == MppItemTypeConsumable)
    {
        for (MppItem& other : Items)
        {
            if (other == item)
            {
                other.Add(item.GetCount());
                return true;
            }
        }
    }
    if (Items.size() < MaxItems)
    {
        Items.push_back(item);
        return true;
    }
    return false;
}

int MppInventory::GetIndex() const
{
    MppAssert(!Items.empty());
    return Index;
}

const MppItem& MppInventory::Get(int index) const
{
    MppAssert(!Items.empty());
    return Items[index];
}

MppItem MppInventory::Remove(int index, int count)
{
    MppAssert(index < Items.size());
    MppItem& item = Items[index];
    MppItem removedItem = item.Remove(count);
    bool erased = false;
    if (item.GetCount() == 0)
    {
        if (Index <= index)
        {
            OnUpArrow();
        }
        Items.erase(Items.begin() + index);
        erased = true;
    }
    if (erased)
    {
        for (int& slot : Slots)
        {
            if (slot == index)
            {
                slot = kInvalidSlot;
            }
            else if (slot > index)
            {
                slot--;
                MppAssert(slot >= 0);
            }
        }
    }
    return removedItem;
}

const MppItem& MppInventory::GetByID(MppItemID id) const
{
    for (int i = 0; i < Items.size(); i++)
    {
        if (Items[i] == MppItem{id})
        {
            return Get(i);
        }
    }
    return kMppItemInvalid;
}

MppItem MppInventory::RemoveByID(MppItemID id, int count)
{
    for (int i = 0; i < Items.size(); i++)
    {
        if (Items[i] == MppItem{id})
        {
            return Remove(i, count);
        }
    }
    MppAssert(false);
    return kMppItemInvalid;
}

const MppItem& MppInventory::GetBySlot(MppInventorySlot slot) const
{
    int slotIndex = Slots[slot];
    if (slotIndex != kInvalidSlot)
    {
        return Items[slotIndex];
    }
    else
    {
        return kMppItemInvalid;
    }
}

MppInventorySlot MppInventory::GetSlotFromIndex(int index)
{
    for (int i = 0; i < MppInventorySlotCount; i++)
    {
        if (Slots[i] == index)
        {
            return MppInventorySlot(i);
        }
    }
    return MppInventorySlotNone;
}

void MppInventory::SetSlot(MppInventorySlot slot, int index)
{
    MppAssert(index < Items.size());
    Slots[slot] = index;
}

void MppInventory::ResetSlot(MppInventorySlot slot)
{
    MppAssert(Slots[slot] != kInvalidSlot);
    Slots[slot] = kInvalidSlot;
}

void MppInventory::SetMaxItems(int max)
{
    MaxItems = max;
}

void MppInventory::OnAction() 
{
    MppAssert(Focused);
    if (!Items.empty())
    {
        OnActionCallback(Index);
    }
}

void MppInventory::OnDrop() 
{
    MppAssert(Focused);
    if (!Items.empty())
    {
        OnDropCallback(Index);
    }
}

void MppInventory::OnUpArrow() 
{
    MppAssert(Focused);
    Index = std::max(Index - 1, 0);
    Top = std::min(Top, Index);
}

void MppInventory::OnDownArrow() 
{
    MppAssert(Focused);
    Index = std::min(Index + 1, int(Items.size() - 1));
    Top = std::max(Top, Index - GetHeight() / 8 + 1);
}

void MppInventory::OnRender()
{
    Render();
}

void MppInventory::SetOnActionCallback(const std::function<void(int index)>& callback)
{
    OnActionCallback = callback;
}

void MppInventory::SetOnDropCallback(const std::function<void(int index)>& callback)
{
    OnDropCallback = callback;
}

bool MppInventory::IsEmpty() const
{
    return Items.empty();
}

void MppInventory::SetIsFocused(bool focused)
{
    Focused = focused;
}

bool MppInventory::IsFocused() const
{
    return Focused;
}

void MppInventory::Clear()
{
    Items.clear();
    Top = 0;
    Index = 0;
    Slots.fill(kInvalidSlot);
}

void MppInventory::ClearItems()
{
    Items.clear();
}
