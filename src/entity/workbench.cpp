#include <memory>

#include "../input.hpp"
#include "../inventory.hpp"
#include "../log.hpp"
#include "mob.hpp"
#include "workbench.hpp"

void MppWorkbenchEntity::OnAction(MppEntity& instigator)
{
    MppFurnitureEntity::OnAction(instigator);
    if (!instigator.Is<MppMobEntity>())
    {
        MppLog("Instigator wasn't an MppMobEntity");
        return;
    }
    MppMobEntity& mob = instigator.Cast<MppMobEntity>();
    Inventory = mob.GetInventory();
    MppInputSetInteraction(SharedFromThis<MppWorkbenchEntity>());
}

void MppWorkbenchEntity::OnAction()
{
}

void MppWorkbenchEntity::OnUp()
{
    Inventory.lock()->OnUp();
}

void MppWorkbenchEntity::OnDown()
{
    Inventory.lock()->OnDown();
}

void MppWorkbenchEntity::OnHeldUp()
{
    Inventory.lock()->OnHeldUp();
}

void MppWorkbenchEntity::OnHeldDown()
{
    Inventory.lock()->OnHeldDown();
}
