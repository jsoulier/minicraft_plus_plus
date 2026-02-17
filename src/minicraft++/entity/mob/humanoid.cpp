#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/inventory.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/sprite.hpp>
#include <minicraft++/world.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/entity/mob/humanoid.hpp>

void MppHumanoidEntity::Visit(SavepointVisitor& visitor)
{
    MppMobEntity::Visit(visitor);
    visitor(Entity);
    visitor(Riding);
}

void MppHumanoidEntity::Render() const
{
    MppMobEntity::Render();
    Render(Inventory->GetBySlot(MppInventorySlotHelmet));
    Render(Inventory->GetBySlot(MppInventorySlotCuirass));
    Render(Inventory->GetBySlot(MppInventorySlotLeggings));
    Render(Inventory->GetBySlot(MppInventorySlotBoots));
    if (Entity)
    {
        Entity->SetX(X);
        Entity->SetY(Y - Entity->GetPhysicsHeight());
        Entity->Render();
    }
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

void MppHumanoidEntity::DoAction() 
{
    if (GetEntity())
    {
        DropEntity();
    }
    else
    {
        MppMobEntity::DoAction();
    }
}

void MppHumanoidEntity::Equip(int index)
{
    if (Entity)
    {
        std::shared_ptr<MppFurnitureEntity> furniture = Entity->Cast<MppFurnitureEntity>();
        if (furniture)
        {
            if (furniture->IsEmpty())
            {
                Inventory->Add(furniture->GetItemID());
                Entity = nullptr;
                RequestAnimationTick();
            }
            else
            {
                return;
            }
        }
    }
    MppMobEntity::Equip(index);
    const MppItem& item = Inventory->Get(index);
    if (item.GetType() & MppItemTypeArmor)
    {
        if (item.GetType() == MppItemTypeHelmet)
        {
            Inventory->SetSlot(MppInventorySlotHelmet, index);
        }
        else if (item.GetType() == MppItemTypeCuirass)
        {
            Inventory->SetSlot(MppInventorySlotCuirass, index);
        }
        else if (item.GetType() == MppItemTypeLeggings)
        {
            Inventory->SetSlot(MppInventorySlotLeggings, index);
        }
        else if (item.GetType() == MppItemTypeBoots)
        {
            Inventory->SetSlot(MppInventorySlotBoots, index);
        }
        else
        {
            MppAssert(false);
        }
    }
    else if (item.GetType() == MppItemTypeFurniture)
    {
        std::shared_ptr<MppEntity> entity = item.CreateFurnitureEntity();
        Inventory->Remove(index);
        PickupEntity(entity);
    }
}

int MppHumanoidEntity::GetActionOffset() const
{
    return 16;
}

void MppHumanoidEntity::PickupEntity(const std::shared_ptr<MppEntity>& entity)
{
    MppAssert(!Entity);
    Entity = entity;
    Entity->Unspawn();
    RequestAnimationTick();
}

void MppHumanoidEntity::DropEntity()
{
    MppAssert(Entity);
    int heldX = Entity->GetX();
    int heldY = Entity->GetY();
    Entity->SetX(X + GetFacingX() * GetSize());
    Entity->SetY(Y + GetFacingY() * GetSize());
    if (Entity->IsColliding())
    {
        Entity->SetX(heldX);
        Entity->SetY(heldY);
        return;
    }
    MppWorldAddEntity(Entity);
    Entity = nullptr;
    RequestAnimationTick();
}

std::shared_ptr<MppEntity> MppHumanoidEntity::GetEntity() const
{
    return Entity;
}

void MppHumanoidEntity::SetRiding(bool riding)
{
    Riding = riding;
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
        Animation.GetMod(),
        MppRendererLayerTopEntity);
}

int MppHumanoidEntity::GetMaxItems() const
{
    return 32;
}

int MppHumanoidEntity::GetAnimationPose() const
{
    if (Riding)
    {
        return 2;
    }
    else if (Entity)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int MppHumanoidEntity::GetAnimationPose1X() const
{
    return 0;
}

int MppHumanoidEntity::GetAnimationPose1Y() const
{
    return 6;
}

int MppHumanoidEntity::GetAnimationPose2X() const
{
    return 4;
}

int MppHumanoidEntity::GetAnimationPose2Y() const
{
    return 6;
}

int MppHumanoidEntity::GetAnimationPose3X() const
{
    return 8;
}

int MppHumanoidEntity::GetAnimationPose3Y() const
{
    return 6;
}
