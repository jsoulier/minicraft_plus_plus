#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

#include "../../assert.hpp"
#include "../../color.hpp"
#include "../../console.hpp"
#include "../../inventory.hpp"
#include "../../log.hpp"
#include "../../renderer.hpp"
#include "../../tile.hpp"
#include "../../world.hpp"
#include "../furniture.hpp"
#include "../item.hpp"
#include "../mob.hpp"
#include "../player.hpp"
#include "player.hpp"

static constexpr int kActionOffset = 12;
static constexpr int kDropDistance = 16;

void MppPlayerController::OnAddEntity()
{
    GetInventory()->SetOnActionCallback(std::bind(&MppPlayerController::OnActionCallback, this, std::placeholders::_1));
    GetInventory()->SetOnDropCallback(std::bind(&MppPlayerController::OnDropCallback, this, std::placeholders::_1));
    MppInputSetPlayer(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
}

void MppPlayerController::OnUpdate(uint64_t ticks)
{
    std::shared_ptr<MppPlayerEntity> player = GetPlayer();
    player->SetCrouching(false);
}

void MppPlayerController::OnAction()
{
    std::shared_ptr<MppPlayerEntity> player = GetPlayer();
    if (player->IsHoldingEntity())
    {
        player->DropHeldEntity();
        return;
    }
    std::vector<std::shared_ptr<MppEntity>> entities = MppWorldGetEntities(player->GetX(), player->GetY());
    std::erase_if(entities, [&](std::shared_ptr<MppEntity>& other)
    {
        // TODO: remove e.g. items
        return player == other;
    });
    int centerX1;
    int centerY1;
    int centerX2;
    int centerY2;
    player->GetCenter(centerX1, centerY1);
    player->PushNow(kActionOffset * player->GetFacingX(), kActionOffset * player->GetFacingY());
    player->GetCenter(centerX2, centerY2);
    if (MppConsole::CVarAction.GetBool())
    {
        MppRendererDrawLine(kMppColorDebugAction, centerX1, centerY1, centerX2, centerY2, MppRendererLayerDebugAction);
    }
    std::sort(entities.begin(), entities.end(), [&player](std::shared_ptr<MppEntity>& lhs, std::shared_ptr<MppEntity>& rhs)
    {
        // TODO: sort mobs in front of furniture
        return player->GetDistance(lhs) < player->GetDistance(rhs);
    });
    bool didAction = false;
    if (!entities.empty())
    {
        std::shared_ptr<MppEntity>& entity = entities[0];
        if (player->GetDistance(entity) <= player->GetActionRange())
        {
            player->DoAction(entity);
            didAction = true;
        }
    }
    if (!didAction)
    {
        int tileX1 = centerX1 / MppTile::kSize;
        int tileY1 = centerY1 / MppTile::kSize;
        int tileX2 = centerX2 / MppTile::kSize;
        int tileY2 = centerY2 / MppTile::kSize;
        MppTile& tile1 = MppWorldGetTile(tileX1, tileY1);
        MppTile& tile2 = MppWorldGetTile(tileX2, tileY2);
        int size = MppTile::kSize;
        if (tile1.IsValid() && tile1.OnAction(*player))
        {
            if (MppConsole::CVarAction.GetBool())
            {
                MppRendererDrawRect(kMppColorDebugAction, tileX1 * size, tileY1 * size, size, size, MppRendererLayerDebugAction);
            }
        }
        else if (tile2.IsValid() && tile2.OnAction(*player))
        {
            if (MppConsole::CVarAction.GetBool())
            {
                MppRendererDrawRect(kMppColorDebugAction, tileX2 * size, tileY2 * size, size, size, MppRendererLayerDebugAction);
            }
        }
        else if (tile1.IsValid())
        {
            // if no preferred tool
        }
    }
    player->PushNow(-kActionOffset * player->GetFacingX(), -kActionOffset * player->GetFacingY());
}

void MppPlayerController::OnInventory()
{
    MppInputSetInteraction(GetPlayer()->GetInventory());
}

void MppPlayerController::OnHeldUp()
{
    GetPlayer()->Push(0, -1);
}

void MppPlayerController::OnHeldDown()
{
    GetPlayer()->Push(0, 1);
}

void MppPlayerController::OnHeldLeft()
{
    GetPlayer()->Push(-1, 0);
}

void MppPlayerController::OnHeldRight()
{
    GetPlayer()->Push(1, 0);
}

void MppPlayerController::OnHeldCrouch()
{
    GetPlayer()->SetCrouching(true);
}

std::shared_ptr<MppPlayerEntity> MppPlayerController::GetPlayer()
{
    return std::dynamic_pointer_cast<MppPlayerEntity>(Entity.lock());
}
        
void MppPlayerController::OnActionCallback(int index)
{
    std::shared_ptr<MppPlayerEntity> player = GetPlayer();
    std::shared_ptr<MppInventory> inventory = player->GetInventory();
    const MppItem& item = inventory->Get(index);
    if (item.GetType() & MppItemTypeEquipment)
    {
        MppInventorySlot slot = inventory->GetSlotFromIndex(index);
        if (slot != MppInventorySlotNone)
        {
            inventory->ResetSlot(slot);
            return;
        }
    }
    if (item.GetType() == MppItemTypeNone)
    {
        MppLog("Tried an action on an MppItemTypeNone: %s", item.GetName().data());
        return;
    }
    else if (item.GetType() & MppItemTypeArmor)
    {
        if (item.GetType() == MppItemTypeHelmet)
        {
            inventory->SetSlot(MppInventorySlotHelmet, index);
        }
        else if (item.GetType() == MppItemTypeCuirass)
        {
            inventory->SetSlot(MppInventorySlotCuirass, index);
        }
        else if (item.GetType() == MppItemTypeLeggings)
        {
            inventory->SetSlot(MppInventorySlotLeggings, index);
        }
        else if (item.GetType() == MppItemTypeBoots)
        {
            inventory->SetSlot(MppInventorySlotBoots, index);
        }
        else
        {
            MppAssert(false);
        }
    }
    else if (item.GetType() == MppItemTypeConsumable)
    {
        // TODO:
    }
    else if (item.GetType() == MppItemTypeFurniture)
    {
        if (!player->IsHoldingEntity())
        {
            std::shared_ptr<MppEntity> entity = item.CreateFurnitureEntity();
            inventory->Remove(index);
            player->Pickup(entity);
        }
    }
    else if (item.GetType() & MppItemTypeTool)
    {
        if (!player->IsHoldingEntity())
        {
            inventory->SetSlot(MppInventorySlotHeld, index);
        }
    }
    else
    {
        MppAssert(false);
    }
}

void MppPlayerController::OnDropCallback(int index)
{
    std::shared_ptr<MppPlayerEntity> player = GetPlayer();
    std::shared_ptr<MppEntity> item = std::make_shared<MppItemEntity>(GetInventory()->Remove(index));
    item->SetX(player->GetX() + player->GetFacingX() * kDropDistance);
    item->SetY(player->GetY() + player->GetFacingY() * kDropDistance);
    MppWorldAddEntity(item);
}
