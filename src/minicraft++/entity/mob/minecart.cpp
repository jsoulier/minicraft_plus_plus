#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/entity/mob/humanoid.hpp>
#include <minicraft++/entity/mob/minecart.hpp>
#include <minicraft++/entity/mob/mount.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/util.hpp>
#include <minicraft++/world.hpp>

void MppMinecartEntity::Render() const 
{
    MppVehicleEntity::Render();
    static constexpr int kColor = 553;
    static constexpr int kRadius = 16;
    static constexpr int kStrength = 5;
    auto [x, y] = GetCenter();
    MppRendererDrawLight(kColor, x, y, kRadius, kStrength);
}

MppItemID MppMinecartEntity::GetItemID() const 
{
    return MppItemIDMinecart;
}

MppTilePhysicsType MppMinecartEntity::GetPhysicsType() const
{
    return MppTilePhysicsTypeRails;
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
