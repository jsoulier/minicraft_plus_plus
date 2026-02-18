#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/entity/mob/horse.hpp>
#include <minicraft++/entity/mob/humanoid.hpp>
#include <minicraft++/entity/mob/minecart.hpp>
#include <minicraft++/inventory.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/sprite.hpp>
#include <minicraft++/world.hpp>

MppHumanoidEntity::MppHumanoidEntity()
    : LocomotionMode{MppHumanoidEntityLocomotionModeDefault}
{
}

void MppHumanoidEntity::Visit(SavepointVisitor& visitor)
{
    MppMobEntity::Visit(visitor);
    visitor(Entity);
    visitor(LocomotionMode);
    if (visitor.IsReading() && Entity)
    {
        Entity->OnCreate();
    }
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
        int size = Entity->GetSize();
        // Top of opaque sprite minus held entity's size, but trimming off top transparent portion
        Entity->SetY(GetPhysicsY() - size + (size - Entity->GetPhysicsHeight()) / 2);
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

void MppHumanoidEntity::OnMount(const std::shared_ptr<MppMobEntity>& vehicle)
{
    if (vehicle->IsA<MppMinecartEntity>())
    {
        LocomotionMode = MppHumanoidEntityLocomotionModeMinecart;
    }
    else if (vehicle->IsA<MppHorseEntity>())
    {
        LocomotionMode = MppHumanoidEntityLocomotionModeHorse;
    }
    else
    {
        MppAssert(false);
    }
    RequestAnimationTick();
}

void MppHumanoidEntity::OnUnmount()
{
    LocomotionMode = MppHumanoidEntityLocomotionModeDefault;
    RequestAnimationTick();
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
    bool droppedEntity = false;
    if (Entity)
    {
        if (Entity->IsEmpty())
        {
            Inventory->Add(Entity->GetItemID());
            Entity = nullptr;
            RequestAnimationTick();
            droppedEntity = true;
        }
        else
        {
            return;
        }
    }
    // There's an annoying edge case when you place a furniture/mob back in your inventory.
    // We want to unequip the furniture/mob and equip the newly selected item. But, we don't
    // want to do the action if the new item is already equipped (because that'll unequip it).
    bool doMobEquip = true;
    for (int i = 0; i < MppInventorySlotCount; i++)
    {
        int slotIndex = Inventory->GetIndexFromSlot(MppInventorySlot(i));
        if (slotIndex == index)
        {
            doMobEquip = false;
        }
    }
    if (doMobEquip || !droppedEntity)
    {
        MppMobEntity::Equip(index);
    }
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
    else if (item.GetType() == MppItemTypeMob)
    {
        std::shared_ptr<MppEntity> entity = item.CreateMobEntity();
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
    if (Entity->Drop(Cast<MppEntity>()))
    {
        MppWorldAddEntity(Entity);
        Entity = nullptr;
        RequestAnimationTick();
    }
}

std::shared_ptr<MppEntity> MppHumanoidEntity::GetEntity() const
{
    return Entity;
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
    if (Entity)
    {
        return 1;
    }
    switch (LocomotionMode)
    {
    case MppHumanoidEntityLocomotionModeHorse: return 2;
    case MppHumanoidEntityLocomotionModeDefault: return 0;
    case MppHumanoidEntityLocomotionModeMinecart: return 3;
    }
    MppAssert(false);
    return 0;
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

int MppHumanoidEntity::GetAnimationPose4X() const
{
    return 0;
}

int MppHumanoidEntity::GetAnimationPose4Y() const
{
    return 21;
}
