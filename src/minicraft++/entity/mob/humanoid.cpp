#include <savepoint/savepoint.hpp>

#include <cstdint>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/inventory.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/sprite.hpp>
#include <minicraft++/world.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/entity/mob/humanoid.hpp>

void MppHumanoidEntity::OnAddEntity()
{
    MppMobEntity::OnAddEntity();
}

void MppHumanoidEntity::Visit(SavepointVisitor& visitor)
{
    MppMobEntity::Visit(visitor);
    visitor(HeldEntity);
}

void MppHumanoidEntity::Render() const
{
    MppMobEntity::Render();
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
        MppAssert(!item.IsValid());
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

void MppHumanoidEntity::HoldEntity(const std::shared_ptr<MppEntity>& entity)
{
    MppAssert(!HeldEntity);
    HeldEntity = entity;
    HeldEntity->Kill();
    SetTickAnimation();
}

void MppHumanoidEntity::DropHeldEntity()
{
    MppAssert(HeldEntity);
    int heldX = HeldEntity->GetX();
    int heldY = HeldEntity->GetY();
    HeldEntity->SetX(X + GetFacingX() * GetSize());
    HeldEntity->SetY(Y + GetFacingY() * GetSize());
    if (!HeldEntity->IsColliding())
    {
        HeldEntity->SetX(heldX);
        HeldEntity->SetY(heldY);
        // TODO: need to play a sound
        return;
    }
    MppWorldAddEntity(HeldEntity);
    HeldEntity = nullptr;
    SetTickAnimation();
}

void MppHumanoidEntity::MoveHeldEntityToInventory()
{
    MppAssert(HeldEntity);
    std::shared_ptr<MppFurnitureEntity> furniture = std::dynamic_pointer_cast<MppFurnitureEntity>(HeldEntity);
    MppAssert(furniture->IsEmpty());
    Inventory->Add(furniture->GetItemID());
    HeldEntity = nullptr;
    SetTickAnimation();
}

std::shared_ptr<MppEntity> MppHumanoidEntity::GetHeldEntity() const
{
    return HeldEntity;
}

void MppHumanoidEntity::DoAction(std::shared_ptr<MppEntity>& entity)
{
    MppAssert(!HeldEntity);
    MppMobEntity::DoAction(entity);
}

int MppHumanoidEntity::GetPose() const
{
    if (HeldEntity)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
