#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/entity/mob/humanoid.hpp>
#include <minicraft++/entity/mob/boat.hpp>
#include <minicraft++/entity/mob/mount.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/util.hpp>
#include <minicraft++/world.hpp>

MppItemID MppBoatEntity::GetItemID() const 
{
    return MppItemIDBoat;
}

MppTilePhysicsType MppBoatEntity::GetPhysicsType() const
{
    return MppTilePhysicsTypeLiquid;
}

int MppBoatEntity::GetAnimationPose1X() const 
{
    return 12;
}

int MppBoatEntity::GetAnimationPose1Y() const 
{
    return 12;
}

int MppBoatEntity::GetColor1() const 
{
    return kMppColorBoat1;
}

int MppBoatEntity::GetColor2() const 
{
    return kMppColorBoat2;
}

int MppBoatEntity::GetColor3() const 
{
    return kMppColorBoat3;
}

int MppBoatEntity::GetColor4() const 
{
    return kMppColorBoat4;
}

int MppBoatEntity::GetColor5() const 
{
    return kMppColorBoat5;
}

int MppBoatEntity::GetPhysicsOffsetX() const 
{
    return 0;
}

int MppBoatEntity::GetPhysicsOffsetY() const 
{
    return 0;
}

int MppBoatEntity::GetPhysicsWidth() const 
{
    return 16;
}

int MppBoatEntity::GetPhysicsHeight() const 
{
    return 16;
}

int MppBoatEntity::GetMoveTickRate() const 
{
    return 2;
}

int MppBoatEntity::GetSpeed() const 
{
    return 1;
}
