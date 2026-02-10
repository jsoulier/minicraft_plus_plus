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
#include <minicraft++/inventory.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/world.hpp>

static constexpr int kDropDistance = 16;

void MppPlayerController::OnAddEntity()
{
    GetInventory()->SetOnActionCallback(std::bind(&MppPlayerController::OnActionCallback, this, std::placeholders::_1));
    GetInventory()->SetOnDropCallback(std::bind(&MppPlayerController::OnDropCallback, this, std::placeholders::_1));
    MppInputSetPlayer(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
}

void MppPlayerController::OnUpdate(uint64_t ticks)
{
    MppController::Update(ticks);
}

void MppPlayerController::OnAction()
{
    Entity.lock()->DoAction();
}

void MppPlayerController::OnInventory()
{
    std::shared_ptr<MppMobEntity> player = Entity.lock();
    std::vector<std::shared_ptr<MppEntity>> entities = MppWorldGetEntities(player->GetX(), player->GetY());
    std::erase_if(entities, [&](std::shared_ptr<MppEntity>& other)
    {
        return std::dynamic_pointer_cast<MppFurnitureEntity>(other) == nullptr;
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
        MppInputSetInteraction(player->GetInventory());
    }
}

void MppPlayerController::OnHeldUp()
{
    Entity.lock()->Push(0, -1);
}

void MppPlayerController::OnHeldDown()
{
    Entity.lock()->Push(0, 1);
}

void MppPlayerController::OnHeldLeft()
{
    Entity.lock()->Push(-1, 0);
}

void MppPlayerController::OnHeldRight()
{
    Entity.lock()->Push(1, 0);
}

void MppPlayerController::OnHeldCrouch()
{
}
        
void MppPlayerController::OnActionCallback(int index)
{
    Entity.lock()->EquipItemFromInventory(index);
}

void MppPlayerController::OnDropCallback(int index)
{
    std::shared_ptr<MppMobEntity> player = Entity.lock();
    std::shared_ptr<MppEntity> item = std::make_shared<MppItemEntity>(GetInventory()->Remove(index));
    item->SetX(player->GetX() + player->GetFacingX() * kDropDistance);
    item->SetY(player->GetY() + player->GetFacingY() * kDropDistance);
    MppWorldAddEntity(item);
}

bool MppPlayerController::ActionFilter(const std::shared_ptr<MppEntity>& entity) const
{
    return !entity->IsA<MppMobEntity>() && !entity->IsA<MppFurnitureEntity>();
}
