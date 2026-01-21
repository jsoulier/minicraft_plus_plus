#include <algorithm>
#include <memory>

#include "../color.hpp"
#include "../input.hpp"
#include "../inventory.hpp"
#include "../item.hpp"
#include "../log.hpp"
#include "chest.hpp"
#include "mob.hpp"

MppChestEntity::MppChestEntity()
    : Inventory{std::make_shared<MppInventory>()}
{
}

void MppChestEntity::OnAction(MppEntity& instigator)
{
    MppFurnitureEntity::OnAction(instigator);
    MppMobEntity* mob = dynamic_cast<MppMobEntity*>(&instigator);
    if (!mob)
    {
        MppLog("Instigator wasn't an MppMobEntity");
        return;
    }
    Other = mob->GetInventory();
    Focused = Inventory;
    MppInputSetInteraction(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
}

void MppChestEntity::OnAction()
{
}

void MppChestEntity::OnUp()
{
    Focused.lock()->OnUp();
}

void MppChestEntity::OnDown()
{
    Focused.lock()->OnDown();
}

void MppChestEntity::OnLeft()
{
    std::swap(Focused, Other);
}

void MppChestEntity::OnRight()
{
    std::swap(Focused, Other);
}

void MppChestEntity::OnHeldUp()
{
    Focused.lock()->OnHeldUp();
}

void MppChestEntity::OnHeldDown()
{
    Focused.lock()->OnHeldDown();
}

MppItemID MppChestEntity::GetItemID() const
{
    return MppItemIDChest;
}

bool MppChestEntity::IsEmpty() const
{
    return Inventory->IsEmpty();
}

int MppChestEntity::GetSpriteX() const
{
    return 2;
}

int MppChestEntity::GetSpriteY() const
{
    return 17;
}

int MppChestEntity::GetColor1() const
{
    return kMppColorChest1;
}

int MppChestEntity::GetColor2() const
{
    return kMppColorChest2;
}

int MppChestEntity::GetColor3() const
{
    return kMppColorChest3;
}

int MppChestEntity::GetColor4() const
{
    return kMppColorChest4;
}

int MppChestEntity::GetColor5() const
{
    return kMppColorChest5;
}
