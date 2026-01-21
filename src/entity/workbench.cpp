#include <memory>

#include "../color.hpp"
#include "../input.hpp"
#include "../inventory.hpp"
#include "../item.hpp"
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

MppItemID MppWorkbenchEntity::GetItemID() const
{
    return MppItemIDWorkbench;
}

int MppWorkbenchEntity::GetSpriteX() const
{
    return 0;
}

int MppWorkbenchEntity::GetSpriteY() const
{
    return 17;
}

int MppWorkbenchEntity::GetColor1() const
{
    return kMppColorWorkbench1;
}

int MppWorkbenchEntity::GetColor2() const
{
    return kMppColorWorkbench2;
}

int MppWorkbenchEntity::GetColor3() const
{
    return kMppColorWorkbench3;
}

int MppWorkbenchEntity::GetColor4() const
{
    return kMppColorWorkbench4;
}

int MppWorkbenchEntity::GetColor5() const
{
    return kMppColorWorkbench5;
}
