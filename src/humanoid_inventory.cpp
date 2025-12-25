#include <savepoint.hpp>

#include "humanoid_inventory.hpp"

MppHumanoidInventory::MppHumanoidInventory()
    : MppInventory()
{
}

void MppHumanoidInventory::Visit(SavepointVisitor& visitor)
{
}

void MppHumanoidInventory::Action()
{
    // TODO: remove
    if (Index != -1)
    {
        Remove(Index);
    }
}
