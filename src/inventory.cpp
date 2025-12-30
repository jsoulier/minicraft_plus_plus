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

void MppInventory::Draw(MppRenderer& renderer)
{
    MppMenuList::Draw(renderer);
}

void MppInventory::Visit(SavepointVisitor& visitor)
{
    MppMenuList::Visit(visitor);
    // TODO: remove
    if (visitor.IsReading())
    {
        Items.clear();
    }
    visitor(Items);
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
    // For derived class (e.g. MppMobInventory::Slots)
    RemoveInternal(index);
    MppMenuList::Remove(index);
    Items.erase(Items.begin() + index);
}

bool MppInventory::Remove(MppItemID id)
{
    for (int i = 0; i < Items.size(); i++)
    {
        if (Items[i].GetID() == id)
        {
            Remove(i);
            return true;
        }
    }
    return false;
}

const MppItem* MppInventory::GetItem() const
{
    if (Index != kInvalidIndex)
    {
        return GetItem(Index);
    }
    else
    {
        SDL_Log("Tried to get item without one currently selected");
        return nullptr;
    }
}

const MppItem* MppInventory::GetItem(int index) const
{
    return &Items.at(index);
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
    return 144;
}

int MppInventory::GetHeight() const
{
    return 112;
}

int MppInventory::GetRowHeight() const
{
    return 8;
}
