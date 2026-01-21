#include "inventory.hpp"

void MppInventory::Visit(SavepointVisitor& visitor)
{
    visitor(Items);
}

void MppInventory::Render()
{
    MppMenu::Render();
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
