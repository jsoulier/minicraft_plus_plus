#include <SDL3/SDL.h>
#include <savepoint.hpp>

#include <string_view>

#include "inventory.hpp"
#include "item.hpp"
#include "renderer.hpp"

static constexpr int kCharacterWidth = 8;

MppInventory::MppInventory()
    : MppMenuList()
{
    // TODO: remove
    Add(MppItem{MppItemIDIronArmor});
    Add(MppItem{MppItemIDIronArmor});
    Add(MppItem{MppItemIDWood});
    Add(MppItem{MppItemIDWood});
}

void MppInventory::Draw(MppRenderer& renderer)
{
    MppMenuList::Draw(renderer);
}

void MppInventory::Draw(MppRenderer& renderer, int y, int index)
{
    const MppItem& item = Items[index];
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
        GetContentX(),
        y,
        false,
        MppRenderer::LayerScreenContent);
    if (item.GetItems())
    {
        // TODO: number characters
        int stringX = GetContentX() + MppItem::kWidth + kCharacterWidth / 2;
        MppMenu::Draw(renderer, "A", kMppColorWhite, stringX, y);
    }
    int stringOffsetX = MppItem::kWidth + kCharacterWidth + kCharacterWidth / 2;
    int stringX = GetContentX() + stringOffsetX + (GetContentWidth() - stringOffsetX) / 2;
    MppMenu::Draw(renderer, item.GetName(), kMppColorWhite, stringX, y);
}

void MppInventory::Visit(SavepointVisitor& visitor)
{
    visitor(Items);
}

void MppInventory::Add(const MppItem& item)
{
    if (item.GetType() & MppItemTypeStackable)
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
    if (item.GetType() & MppItemTypeStackable)
    {
        item.RemoveItem();
        if (item.GetItems())
        {
            return;
        }
    }
    MppMenuList::Remove(index);
    Items.erase(Items.begin() + index);
}

std::string_view MppInventory::GetName() const
{
    return "INVENTORY";
}

int MppInventory::GetX() const
{
    return 16;
}

int MppInventory::GetY() const
{
    return 16;
}

int MppInventory::GetWidth() const
{
    return 112;
}

int MppInventory::GetHeight() const
{
    return 112;
}

int MppInventory::GetRowHeight() const
{
    return 8;
}
