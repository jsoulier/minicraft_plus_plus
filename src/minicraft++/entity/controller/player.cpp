#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/console.hpp>
#include <minicraft++/entity/controller/player.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/entity/item.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/entity/mob/player.hpp>
#include <minicraft++/inventory.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/world.hpp>

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
    GetPlayer()->DoAction();
}

void MppPlayerController::OnInventory()
{
    std::shared_ptr<MppPlayerEntity> player = GetPlayer();
    std::vector<std::shared_ptr<MppEntity>> entities = MppWorldGetEntities(player->GetX(), player->GetY());
    std::erase_if(entities, [&](std::shared_ptr<MppEntity>& other)
    {
        return !bool(std::dynamic_pointer_cast<MppFurnitureEntity>(other));
    });
    bool didInteraction = false;
    for (std::shared_ptr<MppEntity>& entity : entities)
    {
        std::shared_ptr<MppFurnitureEntity> furniture = std::static_pointer_cast<MppFurnitureEntity>(entity);
        if (player->GetDistance(entity) <= player->GetActionRange())
        {
            didInteraction = furniture->OnInteraction(*player);
            if (didInteraction)
            {
                break;
            }
        }
    }
    if (!didInteraction)
    {
        MppInputSetInteraction(GetPlayer()->GetInventory());
    }
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
    std::shared_ptr<MppFurnitureEntity> furniture = std::dynamic_pointer_cast<MppFurnitureEntity>(player->GetHeldEntity());
    if (furniture)
    {
        if (furniture->IsEmpty())
        {
            player->MoveHeldEntityToInventory();
        }
        else
        {
            // TODO: need to play an error noise here
            return;
        }
    }
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
        if (!player->GetHeldEntity())
        {
            std::shared_ptr<MppEntity> entity = item.CreateFurnitureEntity();
            inventory->Remove(index);
            player->HoldEntity(entity);
        }
    }
    else if (item.GetType() & MppItemTypeTool)
    {
        if (!player->GetHeldEntity())
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

bool MppPlayerController::ActionFilter(const std::shared_ptr<MppEntity>& entity) const
{
    return !entity->IsA<MppMobEntity>() && !entity->IsA<MppFurnitureEntity>();
}
