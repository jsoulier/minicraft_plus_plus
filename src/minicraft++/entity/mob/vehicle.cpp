#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/entity/mob/humanoid.hpp>
#include <minicraft++/entity/mob/mount.hpp>
#include <minicraft++/entity/mob/vehicle.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/util.hpp>
#include <minicraft++/world.hpp>

MppVehicleEntity::MppVehicleEntity()
    : MppMobEntity()
    , Mount{std::make_shared<MppMobEntityMount>()}
{
}

void MppVehicleEntity::OnAdd() 
{
    MppMobEntity::OnAdd();
    Mount->OnAdd();
}

void MppVehicleEntity::Visit(SavepointVisitor& visitor) 
{
    MppMobEntity::Visit(visitor);
    visitor(Mount);
}

void MppVehicleEntity::Update(uint64_t ticks) 
{
    MppMobEntity::Update(ticks);
    Mount->Update(ticks);
}

void MppVehicleEntity::Render() const 
{
    MppMobEntity::Render();
    Mount->Render();
}

bool MppVehicleEntity::OnAction(std::shared_ptr<MppEntity>& instigator)
{
    std::shared_ptr<MppHumanoidEntity> humanoid = instigator->Cast<MppHumanoidEntity>();
    MppAssert(humanoid);
    humanoid->PickupEntity(shared_from_this());
    return true;
}

bool MppVehicleEntity::OnInteraction(std::shared_ptr<MppEntity>& instigator) 
{
    Mount->Possess(Cast<MppMobEntity>(), instigator);
    return true;
}

MppEntityCollision MppVehicleEntity::OnCollision(std::shared_ptr<MppEntity>& instigator, int dx, int dy)
{
    if (instigator->IsA<MppMobEntity>() || instigator->IsA<MppFurnitureEntity>())
    {
        int x = X;
        int y = Y;
        Move(dx, dy);
        // If still colliding with instigator, instigator is inside the minecart
        if (IsColliding(instigator))
        {
            X = x;
            Y = y;
        }
    }
    return MppEntityCollisionAccept;
}

void MppVehicleEntity::DoAction() 
{
    Mount->DoAction();
}

bool MppVehicleEntity::Drop(const std::shared_ptr<MppEntity>& instigator) 
{
    std::shared_ptr<MppHumanoidEntity> humanoid = instigator->Cast<MppHumanoidEntity>();
    MppAssert(humanoid);
    int humanoidX = humanoid->GetX();
    int humanoidY = humanoid->GetY();
    auto [x1, y1] = humanoid->GetCenter();
    humanoid->SetX(humanoid->GetX() + humanoid->GetActionOffset() * humanoid->GetFacingX());
    humanoid->SetY(humanoid->GetY() + humanoid->GetActionOffset() * humanoid->GetFacingY());
    auto [x2, y2] = humanoid->GetCenter();
    humanoid->SetX(humanoidX);
    humanoid->SetY(humanoidY);
    int size = MppTile::kSize;
    int tileX1 = x1 / size;
    int tileY1 = y1 / size;
    int tileX2 = x2 / size;
    int tileY2 = y2 / size;
    MppTile& tile1 = MppWorldGetTile(tileX1, tileY1);
    MppTile& tile2 = MppWorldGetTile(tileX2, tileY2);
    if (tile2.GetPhysicsType() == GetPhysicsType())
    {
        X = tileX2 * size;
        Y = tileY2 * size;
        return true;
    }
    if (tile1.GetPhysicsType() == GetPhysicsType())
    {
        X = tileX1 * size;
        Y = tileY1 * size;
        return true;
    }
    return false;
}
