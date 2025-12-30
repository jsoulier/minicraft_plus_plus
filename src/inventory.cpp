#include <SDL3/SDL.h>
#include <savepoint/visitor.hpp>

#include <algorithm>
#include <memory>
#include <string_view>

#include "inventory.hpp"
#include "item.hpp"
#include "renderer.hpp"

static constexpr int kInvalidIndex = -1;
static constexpr int kCharacterWidth = 8;
static constexpr int kSpacing = kCharacterWidth;

MppInventory::Slot::Slot()
    : Index{kInvalidIndex}
{
}

void MppInventory::Slot::Visit(SavepointVisitor& visitor)
{
    visitor(Index);
}

bool MppInventory::Slot::operator==(const Slot other) const
{
    return Index == other.Index;
}

bool MppInventory::Slot::IsValid() const
{
    return Index != kInvalidIndex;
}

MppInventory::MppInventory(int slots)
    : MppMenuList()
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
    Add(MppItem{MppItemIDIronChestplate});
    Add(MppItem{MppItemIDIronChestplate});
    Add(MppItem{MppItemIDIronChestplate});
    Add(MppItem{MppItemIDIronChestplate});
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

void MppInventory::Draw(MppRenderer& renderer)
{
    MppMenuList::Draw(renderer);
}

void MppInventory::Draw(MppRenderer& renderer, int y, int index)
{
    const MppItem& item = Items[index];
    int width = MppItem::kWidth;
    if (item.GetFlag() & MppItemFlagStackable && item.GetItems())
    {
        std::string string = std::to_string(item.GetItems());
        width += kCharacterWidth * string.size();
        width += kCharacterWidth;
    }
    width += kCharacterWidth * item.GetName().size();
    int x = (GetContentWidth() - width) / 2;
    if (x < 0)
    {
        SDL_Log("Width is greater then content width");
    }
    x += GetContentX();
    renderer.Draw(
        MppSprite{
            item.GetColor1(),
            item.GetColor2(),
            item.GetColor3(),
            item.GetColor4(),
            item.GetColor5(),
            item.GetSpriteX(),
            item.GetSpriteY(),
            MppItem::kWidth
        },
        x,
        y,
        false,
        MppRenderer::LayerScreenContent);
    x += MppItem::kWidth;
    if (item.GetFlag() & MppItemFlagStackable && item.GetItems())
    {
        std::string string = std::to_string(item.GetItems());
        x += kCharacterWidth * string.size() / 2;
        MppMenu::Draw(renderer, string, kMppColorText, x, y);
        x += kCharacterWidth * string.size() / 2;
        x += kSpacing;
    }
    x += kCharacterWidth * item.GetName().size() / 2;
    MppMenu::Draw(renderer, item.GetName(), kMppColorText, x, y);
    x = GetContentX();
    if (index == Index)
    {
        MppMenu::Draw(renderer, ">", kMppColorText, x, y);
        MppMenu::Draw(renderer, "<", kMppColorText, x + GetContentWidth(), y);
    }
    // TODO: remove nested loop
    for (const Slot& slot : Slots)
    {
        if (slot.Index == index)
        {
            MppMenu::Draw(renderer, "E", kMppColorText, x + 8, y);
        }
    }
}

void MppInventory::Visit(SavepointVisitor& visitor)
{
    MppMenuList::Visit(visitor);
    // TODO: remove
    if (visitor.IsReading())
    {
        Items.clear();
    }
    if (visitor.IsReading())
    {
        Slots.clear();
    }
    visitor(Items);
    visitor(Slots);
}

void MppInventory::Add(const MppItem& item)
{
    if (item.GetFlag() & MppItemFlagStackable)
    {
        for (int i = 0; i < Items.size(); i++)
        {
            if (item == Items[i])
            {
                Items[i].AddItem();
                return;
            }
        }
    }
    MppMenuList::Add();
    Items.push_back(item);
}

void MppInventory::Remove(int index)
{
    MppItem& item = Items[index];
    if (item.GetFlag() & MppItemFlagStackable)
    {
        item.RemoveItem();
        if (item.GetItems())
        {
            return;
        }
    }
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
    MppMenuList::Remove(index);
    Items.erase(Items.begin() + index);
}

const MppItem* MppInventory::GetItem() const
{
    if (Index != kInvalidIndex)
    {
        return &Items[Index];
    }
    else
    {
        return nullptr;
    }
}

const MppItem* MppInventory::GetItem(int slot) const
{
    if (Slots[slot].IsValid())
    {
        return &Items[Slots[slot].Index];
    }
    else
    {
        return nullptr;
    }
}

void MppInventory::SetSlot(int slot)
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

std::string_view MppInventory::GetName() const
{
    return "INVENTORY";
}

int MppInventory::GetX() const
{
    return 4;
}

int MppInventory::GetY() const
{
    return 16;
}

int MppInventory::GetWidth() const
{
    return 160;
}

int MppInventory::GetHeight() const
{
    return 112;
}

int MppInventory::GetRowHeight() const
{
    return 8;
}
