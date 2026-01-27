#include <savepoint/savepoint.hpp>

#include <cmath>
#include <cstdint>

#include "../assert.hpp"
#include "../color.hpp"
#include "../console.hpp"
#include "../renderer.hpp"
#include "../sprite.hpp"
#include "../tile.hpp"
#include "../util.hpp"
#include "../world.hpp"
#include "creature.hpp"
#include "mob.hpp"

static constexpr int kTickRate = 10;
static constexpr int kMaxMoveDistance = 6;

MppCreatureEntity::MppCreatureEntity()
    : MppMobEntity()
    , Animation{}
    , State{MppCreatureEntityStateIdle}
    , TargetX{}
    , TargetY{}
    , IdleTicks{}
    , FleeTicks{}
{
}

void MppCreatureEntity::OnAddEntity()
{
    MppMobEntity::OnAddEntity();
    Animation.SetTickRate(kTickRate);
    Animation.SetPose(0, GetSpritePoseX(), GetSpritePoseY());
    // TODO: having to update here is error prone
    Animation.Update(0, FacingX, FacingY, 0);
}

void MppCreatureEntity::Visit(SavepointVisitor& visitor)
{
    MppMobEntity::Visit(visitor);
    visitor(State);
    visitor(TargetX);
    visitor(TargetY);
    visitor(IdleTicks);
    visitor(FleeTicks);
}

void MppCreatureEntity::Update(uint64_t ticks)
{
    IdleTicks--;
    FleeTicks--;
    Animation.Update(0, VelocityX, VelocityY, ticks);
    MppMobEntity::Update(ticks);
    switch (State)
    {
    case MppCreatureEntityStateIdle: Idle(); break;
    case MppCreatureEntityStateMove: Move(); break;
    default: MppAssert(false);
    }
}

void MppCreatureEntity::Render() const
{
    MppMobEntity::Render();
    MppRendererDraw(
        MppSprite{
            GetSpriteColor1(),
            GetSpriteColor2(),
            GetSpriteColor3(),
            GetSpriteColor4(),
            GetSpriteColor5(),
            Animation.GetX(),
            Animation.GetY(),
            GetSize(),
        },
        X,
        Y,
        Animation.GetFlip(),
        MppRendererLayerEntity);
}

void MppCreatureEntity::OnAction(MppEntity& instigator)
{
    MppMobEntity::OnAction(instigator);
    // TODO: set FleeTicks here
}

int MppCreatureEntity::GetSpeed() const
{
    if (FleeTicks <= 0)
    {
        return GetDefaultSpeed();
    }
    else
    {
        return GetFleeSpeed();
    }
}

int MppCreatureEntity::GetDefaultSpeed() const
{
    return 3;
}

int MppCreatureEntity::GetFleeSpeed() const
{
    return 1;
}

int MppCreatureEntity::GetIdleCooldown() const
{
    return 120;
}

int MppCreatureEntity::GetFleeCooldown() const
{
    return 300;
}

void MppCreatureEntity::Idle()
{
    if (IdleTicks > 0)
    {
        return;
    }
    MppAssert(State == MppCreatureEntityStateIdle);
    int dx = 0;
    int dy = 0;
    switch (MppGetRandom(0, 3))
    {
    case 0: dx = -1; break;
    case 1: dx = 1; break;
    case 2: dy = -1; break;
    case 3: dy = 1; break;
    }
    int distance = MppGetRandom(1, kMaxMoveDistance);
    int tx = X / MppTile::kSize;
    int ty = Y / MppTile::kSize;
    for (int i = 0; i < distance; i++)
    {
        tx += dx;
        ty += dy;
        const MppTile& tile = MppWorldGetTile(tx, ty);
        if (tile.GetPhysicsType() != MppTilePhysicsTypeGround)
        {
            tx -= dx;
            ty -= dy;
            break;
        }
    }
    TargetX = tx * MppTile::kSize;
    TargetY = ty * MppTile::kSize;
    State = MppCreatureEntityStateMove;
}

void MppCreatureEntity::Move()
{
    MppAssert(State != MppCreatureEntityStateIdle);
    int dx = TargetX - X;
    int dy = TargetY - Y;
    if (!dx && !dy)
    {
        MppAssert(IdleTicks <= 0);
        if (FleeTicks <= 0)
        {
            IdleTicks = GetIdleCooldown();
        }
        State = MppCreatureEntityStateIdle;
        return;
    }
    if (MppConsole::CVarNavigation.GetBool())
    {
        int x1 = X + MppTile::kSize / 2;
        int y1 = Y + MppTile::kSize / 2;
        int x2 = TargetX + MppTile::kSize / 2;
        int y2 = TargetY + MppTile::kSize / 2;
        MppRendererDrawLine(kMppColorDebugNavigation, x1, y1, x2, y2, MppRendererLayerDebugNavigation);
    }
    MppMobEntity::Move(dx, dy);
}
