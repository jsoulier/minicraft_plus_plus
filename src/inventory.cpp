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
    Add(MppItem{MppItemIDIronHelmet});
    Add(MppItem{MppItemIDIronHelmet});
    Add(MppItem{MppItemIDIronHelmet});
    Add(MppItem{MppItemIDIronHelmet});
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
}

void MppInventory::Draw(MppRenderer& renderer)
{
    MppMenuList::Draw(renderer);
}

void MppInventory::Draw(MppRenderer& renderer, int y, int index, bool selected)
{
    const MppItem& item = Items[index];
    int width = MppItem::kWidth;
    if (item.GetFlag() & MppItemFlagStackable && item.GetItems())
    {
        width += kCharacterWidth * item.GetItems();
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
        x += kCharacterWidth;
    }
    x += kCharacterWidth * item.GetName().size() / 2;
    MppMenu::Draw(renderer, item.GetName(), kMppColorText, x, y);
    if (selected)
    {
        x = GetContentX();
        MppMenu::Draw(renderer, ">", kMppColorText, x, y);
        MppMenu::Draw(renderer, "<", kMppColorText, x + GetContentWidth(), y);
    }
}

void MppInventory::Visit(SavepointVisitor& visitor)
{
    visitor(Items);
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
    MppMenuList::Remove(index);
    Items.erase(Items.begin() + index);
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
