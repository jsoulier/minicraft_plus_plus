#include <savepoint/savepoint.hpp>

#include <algorithm>
#include <memory>
#include <vector>

#include "../color.hpp"
#include "../entity.hpp"
#include "../input.hpp"
#include "../inventory.hpp"
#include "../item.hpp"
#include "../renderer.hpp"
#include "../tile.hpp"
#include "../world.hpp"
#include "item.hpp"
#include "player.hpp"

static constexpr int kActionOffset = 8;
static constexpr int kActionDistance = 16;
static constexpr int kDropDistance = 16;

MppPlayerEntity::MppPlayerEntity()
    : MppHumanoidEntity()
{
    Inventory->SetIsFocused(true);
    Inventory->SetX2(124);
    Inventory->SetY2(136);
    Inventory->SetOnActionCallback([this](int index)
    {
        // TODO
    });
    Inventory->SetOnDropCallback([this](int index)
    {
        std::shared_ptr<MppEntity> item = std::make_shared<MppItemEntity>(Inventory->Remove(index));
        item->SetX(X + FacingX * kDropDistance);
        item->SetY(Y + FacingY * kDropDistance);
        MppWorldAddEntity(item);
    });
}

void MppPlayerEntity::OnAddEntity()
{
    MppHumanoidEntity::OnAddEntity();
    MppInputSetPlayer(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
}

void MppPlayerEntity::Update(uint64_t ticks)
{
    MppHumanoidEntity::Update(ticks);
    MppRendererMove(X, Y, GetSize());
}

void MppPlayerEntity::Render() const
{
    MppHumanoidEntity::Render();
}

void MppPlayerEntity::OnAction()
{
    std::vector<std::shared_ptr<MppEntity>> entities = MppWorldGetEntities(X, Y);
    std::erase_if(entities, [&](std::shared_ptr<MppEntity>& other)
    {
        // TODO: remove e.g. items
        return this == other.get();
    });
    X += kActionOffset * FacingX;
    Y += kActionOffset * FacingY;
    std::sort(entities.begin(), entities.end(), [this](std::shared_ptr<MppEntity>& lhs, std::shared_ptr<MppEntity>& rhs)
    {
        // TODO: sort mobs in front of furniture
        return GetDistance(lhs) < GetDistance(rhs);
    });
    if (!entities.empty())
    {
        std::shared_ptr<MppEntity>& entity = entities[0];
        if (GetDistance(entity) <= kActionDistance)
        {
            entity->OnAction(*this);
        }
    }
    else
    {
        // TODO: tiles
    }
    X -= kActionOffset * FacingX;
    Y -= kActionOffset * FacingY;
}

void MppPlayerEntity::OnInventory()
{
    MppInputSetInteraction(Inventory);
}

void MppPlayerEntity::OnHeldUp()
{
    VelocityY--;
}

void MppPlayerEntity::OnHeldDown()
{
    VelocityY++;
}

void MppPlayerEntity::OnHeldLeft()
{
    VelocityX--;
}

void MppPlayerEntity::OnHeldRight()
{
    VelocityX++;
}

int MppPlayerEntity::GetSpriteBorderColor() const
{
    return kMppColorPlayerBorder;
}

int MppPlayerEntity::GetSpriteSkinColor() const
{
    return kMppColorPlayerSkin;
}

int MppPlayerEntity::GetSpriteShirtColor() const
{
    return kMppColorPlayerShirt;
}

int MppPlayerEntity::GetSpritePantColor() const
{
    return kMppColorPlayerPants;
}

int MppPlayerEntity::GetSpriteShoeColor() const
{
    return kMppColorPlayerShoes;
}
