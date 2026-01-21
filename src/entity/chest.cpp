#include <algorithm>
#include <memory>

#include "../input.hpp"
#include "../inventory.hpp"
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
    if (!instigator.Is<MppMobEntity>())
    {
        MppLog("Instigator wasn't an MppMobEntity");
        return;
    }
    MppMobEntity& mob = instigator.Cast<MppMobEntity>();
    Other = mob.GetInventory();
    Focused = Inventory;
    MppInputSetInteraction(SharedFromThis<MppChestEntity>());
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
