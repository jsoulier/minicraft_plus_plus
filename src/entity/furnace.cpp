#include <memory>

#include "../color.hpp"
#include "../input.hpp"
#include "../inventory.hpp"
#include "../item.hpp"
#include "../log.hpp"
#include "furnace.hpp"
#include "mob.hpp"

void MppFurnaceEntity::OnAction(MppEntity& instigator)
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

void MppFurnaceEntity::OnAction()
{
}

void MppFurnaceEntity::OnUp()
{
    Inventory.lock()->OnUp();
}

void MppFurnaceEntity::OnDown()
{
    Inventory.lock()->OnDown();
}

void MppFurnaceEntity::OnHeldUp()
{
    Inventory.lock()->OnHeldUp();
}

void MppFurnaceEntity::OnHeldDown()
{
    Inventory.lock()->OnHeldDown();
}

MppItemID MppFurnaceEntity::GetItemID() const
{
    return MppItemIDFurnace;
}

int MppFurnaceEntity::GetSpriteX() const
{
    return 1;
}

int MppFurnaceEntity::GetSpriteY() const
{
    return 17;
}

int MppFurnaceEntity::GetColor1() const
{
    return kMppColorFurnace1;
}

int MppFurnaceEntity::GetColor2() const
{
    return kMppColorFurnace2;
}

int MppFurnaceEntity::GetColor3() const
{
    return kMppColorFurnace3;
}

int MppFurnaceEntity::GetColor4() const
{
    return kMppColorFurnace4;
}

int MppFurnaceEntity::GetColor5() const
{
    return kMppColorFurnace5;
}
