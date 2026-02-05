#include <savepoint/savepoint.hpp>

#include <cstdint>

#include "../assert.hpp"
#include "../color.hpp"
#include "../inventory.hpp"
#include "../renderer.hpp"
#include "../sprite.hpp"
#include "../world.hpp"
#include "humanoid.hpp"

static constexpr int kTickRate = 10;

void MppHumanoidEntity::OnAddEntity()
{
    MppMobEntity::OnAddEntity();
    Animation.SetTickRate(kTickRate);
    Animation.SetPose(0, GetSpritePose1X(), GetSpritePose1Y());
    Animation.SetPose(1, GetSpritePose2X(), GetSpritePose2Y());
}

void MppHumanoidEntity::Visit(SavepointVisitor& visitor)
{
    MppMobEntity::Visit(visitor);
    visitor(HeldEntity);
}

void MppHumanoidEntity::PostUpdate(uint64_t ticks) 
{
    MppMobEntity::PostUpdate(ticks);
    if (VelocityX || VelocityY)
    {
        if (!HeldEntity)
        {
            Animation.Update(0, FacingX, FacingY, ticks);
        }
        else
        {
            Animation.Update(1, FacingX, FacingY, ticks);
        }
    }
}

void MppHumanoidEntity::Render() const
{
    MppMobEntity::Render();
    MppRendererDraw(
        MppSprite{
            GetSpriteBorderColor(),
            GetSpriteShirtColor(),
            GetSpritePantColor(),
            GetSpriteSkinColor(),
            GetSpriteShoeColor(),
            Animation.GetX(),
            Animation.GetY(),
            GetSize(),
        },
        X,
        Y,
        Animation.GetFlip(),
        MppRendererLayerEntity);
    Render(Inventory->GetBySlot(MppInventorySlotHelmet));
    Render(Inventory->GetBySlot(MppInventorySlotCuirass));
    Render(Inventory->GetBySlot(MppInventorySlotLeggings));
    Render(Inventory->GetBySlot(MppInventorySlotBoots));
    if (HeldEntity)
    {
        HeldEntity->SetX(X);
        HeldEntity->SetY(Y - HeldEntity->GetPhysicsHeight());
        HeldEntity->Render();
    }
}

void MppHumanoidEntity::Render(const MppItem& item) const
{
    int offset = 0;
    if (item.GetType() == MppItemTypeHelmet)
    {
        offset = 1;
    }
    else if (item.GetType() == MppItemTypeCuirass)
    {
        offset = 2;
    }
    else if (item.GetType() == MppItemTypeLeggings)
    {
        offset = 3;
    }
    else if (item.GetType() == MppItemTypeBoots)
    {
        offset = 4;
    }
    else
    {
        return;
    }
    MppRendererDraw(
        MppSprite{
            item.GetColor1(),
            item.GetColor2(),
            item.GetColor3(),
            item.GetColor4(),
            item.GetColor5(),
            Animation.GetX(),
            Animation.GetY() + offset,
            GetSize(),
        },
        X,
        Y,
        Animation.GetFlip(),
        MppRendererLayerEntityOverlay);
}

int MppHumanoidEntity::GetPhysicsOffsetX() const
{
    return 2;
}

int MppHumanoidEntity::GetPhysicsOffsetY() const
{
    return 1;
}

int MppHumanoidEntity::GetPhysicsWidth() const
{
    return 12;
}

int MppHumanoidEntity::GetPhysicsHeight() const
{
    return 14;
}

int MppHumanoidEntity::GetMaxItems() const
{
    return 32;
}

int MppHumanoidEntity::GetSpritePose1X() const
{
    return 0;
}

int MppHumanoidEntity::GetSpritePose1Y() const
{
    return 6;
}

int MppHumanoidEntity::GetSpritePose2X() const
{
    return 4;
}

int MppHumanoidEntity::GetSpritePose2Y() const
{
    return 6;
}

int MppHumanoidEntity::GetActionRange() const
{
    return 16;
}

void MppHumanoidEntity::Pickup(const std::shared_ptr<MppEntity>& entity)
{
    MppAssert(!HeldEntity);
    HeldEntity = entity;
}

bool MppHumanoidEntity::IsHoldingEntity() const
{
    return bool(HeldEntity);
}

void MppHumanoidEntity::DropHeldEntity()
{
    MppAssert(HeldEntity);
    MppWorldAddEntity(HeldEntity);
    HeldEntity = nullptr;
}

void MppHumanoidEntity::DoAction(std::shared_ptr<MppEntity>& entity)
{
    MppAssert(!HeldEntity);
    Animation.Update(0, FacingX, FacingY);
    entity->OnAction(*this);
}
