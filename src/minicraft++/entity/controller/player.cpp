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

void MppPlayerController::Possess(const std::shared_ptr<MppMobEntity>& entity)
{
    MppController::Possess(entity);
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
}

void MppPlayerController::Unpossess()
{
    MppController::Unpossess();
    MppInputRemoveHandler(this);
}

void MppPlayerController::Update(uint64_t ticks)
{
    MppController::Update(ticks);
}

void MppPlayerController::RenderFromEntity() const
{
    MppController::RenderFromEntity();
}

void MppPlayerController::OnAction()
{
    Entity.lock()->DoAction();
}

void MppPlayerController::OnRender() const
{
    std::shared_ptr<MppMobEntity> player = Entity.lock();
    const std::shared_ptr<MppInventory> inventory = GetInventory();
    // TODO: should be at the start of the frame for all entities
    MppRendererMove(player->GetX(), player->GetY(), player->GetSize());
    MppMenu::Render();
    int health = std::ceil(float(player->GetHealth()) / 10);
    int hunger = std::ceil(float(player->GetHunger()) / 10);
    int energy = std::ceil(float(player->GetEnergy()) / 10);
    for (int i = 0; i < health; i++)
    {
        int x = i * MppItem::kSize;
        int y = 128;
        MppItem{MppItemIDHeart}.Render(x, y, MppRendererLayerMenuContent);
    }
    for (int i = 0; i < hunger; i++)
    {
        int x = 176 + i * MppItem::kSize;
        int y = 136;
        MppItem{MppItemIDFood}.Render(x, y, MppRendererLayerMenuContent);
    }
    for (int i = 0; i < energy; i++)
    {
        int x = i * MppItem::kSize;
        int y = 136;
        MppItem{MppItemIDEnergy}.Render(x, y, MppRendererLayerMenuContent);
    }
    const MppItem& held = inventory->GetBySlot(MppInventorySlotHeld);
    std::shared_ptr<MppHumanoidEntity> humanoid = std::dynamic_pointer_cast<MppHumanoidEntity>(player);
    if (humanoid)
    {
        if (humanoid->GetHeldEntity())
        {
            MppMenu::Render(humanoid->GetHeldEntity()->GetName(), kMppColorMenuUnlocked, 256, 128, MppMenuAlignmentRight);
        }
        else if (held.IsValid())
        {
            MppMenu::Render(held.GetName(), kMppColorMenuUnlocked, 256, 128, MppMenuAlignmentRight);
        }
    }
}

void MppPlayerController::OnInteract()
{
    std::shared_ptr<MppMobEntity> player = Entity.lock();
    std::vector<std::shared_ptr<MppEntity>> entities = MppWorldGetEntities(player->GetX(), player->GetY());
    std::erase_if(entities, [&](std::shared_ptr<MppEntity>& other)
    {
        return player == other || InteractionFilter(other);
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
        MppInputAddHandler(player->GetInventory());
    }
}

void MppPlayerController::OnExit()
{
    // No-op. Player controller can't be removed
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

bool MppPlayerController::InteractionFilter(const std::shared_ptr<MppEntity>& entity) const
{
    return !entity->IsA<MppCreatureEntity>() && !entity->IsA<MppFurnitureEntity>();
}
