#include <memory>

#include "../input.hpp"
#include "../inventory.hpp"
#include "../log.hpp"
#include "mob.hpp"
#include "workbench.hpp"

void MppWorkbenchEntity::OnAction(MppEntity& instigator)
{
    MppFurnitureEntity::OnAction(instigator);
    MppMobEntity* mob = dynamic_cast<MppMobEntity*>(&instigator);
    if (!mob)
    {
        MppLog("Instigator wasn't an MppMobEntity");
        return;
    }
    Inventory = mob->GetInventory();
    MppInputSetInteraction(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
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
