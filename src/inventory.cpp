#include <savepoint/visitor.hpp>

#include <limits>
#include <string>

#include "assert.hpp"
#include "color.hpp"
#include "inventory.hpp"
#include "item.hpp"
#include "log.hpp"
#include "menu.hpp"
#include "renderer.hpp"

static constexpr int kInvalidSlot = std::numeric_limits<int>::max();

MppInventory::MppInventory()
    : Items{}
    , Slots{}
    , MaxItems{0}
    , Top{0}
    , Index{0}
{
    Slots.fill(kInvalidSlot);
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
            MppMenu::Render(std::to_string(count), kMppColorMenuForeground, X1 + 16, y, MppMenuAlignmentLeft);
            MppMenu::Render(item.GetName(), kMppColorMenuForeground, X1 + 32, y, MppMenuAlignmentLeft);
        }
        else
        {
            MppMenu::Render(item.GetName(), kMppColorMenuForeground, X1 + 16, y, MppMenuAlignmentLeft);
        }
    }
    MppMenu::Render(">", kMppColorMenuForeground, X1, Y1 + (Index - Top) * 8, MppMenuAlignmentLeft);
}

bool MppInventory::Add(const MppItem& item)
{
    MppAssert(MaxItems > 0);
    if (item.GetType() == MppItemTypeConsumable)
    {
        for (MppItem& other : Items)
        {
            if (other == item)
            {
                other.Add();
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

void MppInventory::SetMaxItems(int max)
{
    MaxItems = max;
}

void MppInventory::OnAction() 
{
}

void MppInventory::OnUpArrow() 
{
    Index = std::max(Index - 1, 0);
    Top = std::min(Top, Index);
}

void MppInventory::OnDownArrow() 
{
    Index = std::min(Index + 1, int(Items.size() - 1));
    Top = std::max(Top, Index - GetHeight() / 8 + 1);
}

void MppInventory::OnHeldUp() 
{
    // TODO: OnUp()
}

void MppInventory::OnHeldDown() 
{
    // TODO: OnHeldDown()
}

void MppInventory::OnRender()
{
    Render();
}

bool MppInventory::IsEmpty() const
{
    return Items.empty();
}
