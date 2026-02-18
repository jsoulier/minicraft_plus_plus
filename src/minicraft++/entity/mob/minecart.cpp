#include <minicraft++/entity/mount.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/entity/mob/humanoid.hpp>
#include <minicraft++/entity/mob/minecart.hpp>
#include <minicraft++/entity/mount.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/util.hpp>
#include <minicraft++/world.hpp>

MppMinecartEntity::MppMinecartEntity()
    : MppMobEntity()
    , Mount{std::make_shared<MppMountEntityProxy>()}
{
}

void MppMinecartEntity::OnAdd() 
{
    MppMobEntity::OnAdd();
    Mount->OnAdd();
}

void MppMinecartEntity::Visit(SavepointVisitor& visitor) 
{
    MppMobEntity::Visit(visitor);
    visitor(Mount);
}

void MppMinecartEntity::Update(uint64_t ticks) 
{
    MppMobEntity::Update(ticks);
    Mount->Update(ticks);
}

void MppMinecartEntity::Render() const 
{
    MppMobEntity::Render();
    Mount->Render();
    static constexpr int kColor = 553;
    static constexpr int kRadius = 16;
    static constexpr int kStrength = 5;
    auto [x, y] = GetCenter();
    MppRendererDrawLight(kColor, x, y, kRadius, kStrength);
}

bool MppMinecartEntity::OnAction(std::shared_ptr<MppEntity>& instigator)
{
    std::shared_ptr<MppHumanoidEntity> humanoid = instigator->Cast<MppHumanoidEntity>();
    MppAssert(humanoid);
    humanoid->PickupEntity(shared_from_this());
    return true;
}

bool MppMinecartEntity::OnInteraction(std::shared_ptr<MppEntity>& instigator) 
{
    Mount->Possess(Cast<MppMobEntity>(), instigator);
    return true;
}

MppEntityCollision MppMinecartEntity::OnCollision(std::shared_ptr<MppEntity>& instigator, int dx, int dy)
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

void MppMinecartEntity::DoAction() 
{
    Mount->DoAction();
}

MppItemID MppMinecartEntity::GetItemID() const 
{
    return MppItemIDMinecart;
}

bool MppMinecartEntity::Drop(const std::shared_ptr<MppEntity>& instigator) 
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
    if (tile2.GetPhysicsType() == MppTilePhysicsTypeRails)
    {
        X = tileX2 * size;
        Y = tileY2 * size;
        return true;
    }
    if (tile1.GetPhysicsType() == MppTilePhysicsTypeRails)
    {
        X = tileX1 * size;
        Y = tileY1 * size;
        return true;
    }
    return false;
}

int MppMinecartEntity::GetAnimationPose1X() const 
{
    return 12;
}

int MppMinecartEntity::GetAnimationPose1Y() const 
{
    return 11;
}

int MppMinecartEntity::GetColor1() const 
{
    return kMppColorIron1;
}

int MppMinecartEntity::GetColor2() const 
{
    return kMppColorIron2;
}

int MppMinecartEntity::GetColor3() const 
{
    return kMppColorIron3;
}

int MppMinecartEntity::GetColor4() const 
{
    return kMppColorIron4;
}

int MppMinecartEntity::GetColor5() const 
{
    return kMppColorIron5;
}

int MppMinecartEntity::GetPhysicsOffsetX() const 
{
    return 0;
}

int MppMinecartEntity::GetPhysicsOffsetY() const 
{
    return 0;
}

int MppMinecartEntity::GetPhysicsWidth() const 
{
    return 16;
}

int MppMinecartEntity::GetPhysicsHeight() const 
{
    return 16;
}

int MppMinecartEntity::GetSpeed() const 
{
    return 2;
}
