#include <savepoint.hpp>

#include <string_view>

#include "inventory.hpp"
#include "renderer.hpp"

void MppInventory::Draw(MppRenderer& renderer)
{
    MppScreen::Draw(renderer);
}

void MppInventory::Visit(SavepointVisitor& visitor)
{
    visitor(Items);
}

std::string_view MppInventory::GetName() const
{
    return "Inventory";
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
