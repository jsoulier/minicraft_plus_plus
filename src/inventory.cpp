#include <savepoint/visitor.hpp>

#include <limits>

#include "inventory.hpp"
#include "item.hpp"

static constexpr int kInvalidSlot = std::numeric_limits<int>::max();

void MppInventory::Visit(SavepointVisitor& visitor)
{
    visitor(Items);
}

void MppInventory::Render() const
{
    MppMenu::Render();
}

bool MppInventory::Add(const MppItem& item)
{
    return false;
}

void MppInventory::OnAction() 
{
}

void MppInventory::OnUp() 
{
}

void MppInventory::OnDown() 
{
}

void MppInventory::OnHeldUp() 
{
}

void MppInventory::OnHeldDown() 
{
}

void MppInventory::OnRender()
{
    Render();
}

bool MppInventory::IsEmpty() const
{
    return Items.empty();
}
