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
#include <minicraft++/entity/mob/creature.hpp>
#include <minicraft++/entity/mob/humanoid.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/inventory.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/world.hpp>

static constexpr int kDropDistance = 16;

bool MppPlayerController::Possess(const std::shared_ptr<MppMobEntity>& entity)
{
    if (!MppController::Possess(entity))
    {
        return false;
    }
    std::shared_ptr<MppInventory> inventory = GetInventory();
    inventory->SetOnActionCallback(std::bind(&MppPlayerController::OnActionCallback, this, std::placeholders::_1));
    inventory->SetOnDropCallback(std::bind(&MppPlayerController::OnDropCallback, this, std::placeholders::_1));
    MppInputAddHandler(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
    SetX1(0);
    SetX2(256);
    SetY1(128);
    SetY2(144);
    inventory->SetIsFocused(true);
    inventory->SetX1(4);
    inventory->SetY1(4);
    inventory->SetX2(126);
    inventory->SetY2(124);
    return true;
}

bool MppPlayerController::Unpossess()
{
    if (!MppController::Unpossess())
    {
        return false;
    }
    MppInputRemoveHandler(this);
    return true;
}

void MppPlayerController::Update(uint64_t ticks)
{
    MppController::Update(ticks);
}

void MppPlayerController::Render() const
{
    MppController::Render();
}

void MppPlayerController::OnInputAction()
{
    Entity.lock()->DoAction();
}

void MppPlayerController::OnInputRender() const
{
    std::shared_ptr<MppMobEntity> player = Entity.lock();
    std::shared_ptr<MppHumanoidEntity> humanoid = player->Cast<MppHumanoidEntity>();
    const std::shared_ptr<MppInventory> inventory = GetInventory();
    MppRendererMove(player->GetX(), player->GetY(), player->GetSize());
    MppMenu::Render();
    int health = std::ceil(float(player->GetHealth()) / 10);
    int hunger = std::ceil(float(player->GetHunger()) / 10);
    int energy = std::ceil(float(player->GetEnergy()) / 10);
    for (int i = 0; i < health; i++)
    {
        int x = i * MppItem::kSize;
        int y = 128;
        MppItem{MppItemIDHeart}.Render(x, y, MppRendererLayerTopMenu);
    }
    for (int i = 0; i < hunger; i++)
    {
        int x = 256 - (i + 1) * MppItem::kSize;
        int y = 136;
        MppItem{MppItemIDFood}.Render(x, y, MppRendererLayerTopMenu);
    }
    for (int i = 0; i < energy; i++)
    {
        int x = i * MppItem::kSize;
        int y = 136;
        MppItem{MppItemIDEnergy}.Render(x, y, MppRendererLayerTopMenu);
    }
    const MppItem& held = inventory->GetBySlot(MppInventorySlotHeld);
    if (humanoid)
    {
        if (humanoid->GetEntity())
        {
            MppMenu::Render(humanoid->GetEntity()->GetName(), kMppColorMenuUnlocked, 256, 128, MppMenuAlignmentRight);
        }
        else if (held.IsValid())
        {
            MppMenu::Render(held.GetName(), kMppColorMenuUnlocked, 256, 128, MppMenuAlignmentRight);
        }
    }
}

void MppPlayerController::OnInputInteract()
{
    std::shared_ptr<MppMobEntity> player = Entity.lock();
    std::shared_ptr<MppEntity> self = player->Cast<MppEntity>();
    std::vector<std::shared_ptr<MppEntity>> entities = MppWorldGetEntities(player->GetX(), player->GetY());
    std::erase_if(entities, [&](std::shared_ptr<MppEntity>& other)
    {
        return player == other || InteractionFilter(other);
    });
    bool didInteraction = false;
    for (std::shared_ptr<MppEntity>& entity : entities)
    {
        if (player->GetDistance(entity) <= player->GetActionOffset())
        {
            didInteraction = entity->OnInteraction(self);
            if (didInteraction)
            {
                break;
            }
        }
    }
    if (!didInteraction)
    {
        MppInputAddHandler(player->GetInventory());
    }
}

void MppPlayerController::OnInputExit()
{
    // No-op. Player controller can't be removed
}

void MppPlayerController::OnInputHeldUp()
{
    Entity.lock()->Push(0, -1, true);
}

void MppPlayerController::OnInputHeldDown()
{
    Entity.lock()->Push(0, 1, true);
}

void MppPlayerController::OnInputHeldLeft()
{
    Entity.lock()->Push(-1, 0, true);
}

void MppPlayerController::OnInputHeldRight()
{
    Entity.lock()->Push(1, 0, true);
}

void MppPlayerController::OnActionCallback(int index)
{
    Entity.lock()->Equip(index);
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

bool MppPlayerController::InteractionFilter(const std::shared_ptr<MppEntity>& entity) const
{
    return !entity->IsA<MppCreatureEntity>() && !entity->IsA<MppFurnitureEntity>();
}
