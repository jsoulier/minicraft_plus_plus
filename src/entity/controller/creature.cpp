#include <savepoint/savepoint.hpp>

#include <cstdint>

#include "../../assert.hpp"
#include "../../color.hpp"
#include "../../console.hpp"
#include "../../renderer.hpp"
#include "../../tile.hpp"
#include "../creature.hpp"
#include "creature.hpp"

static constexpr int kMaxRandomMoveDistance = 6;

MppCreatureController::MppCreatureController(const std::shared_ptr<MppMobEntity>& entity)
    : MppController(entity)
    , State{MppCreatureControllerStateIdle}
    , TargetX{}
    , TargetY{}
    , IdleTicks{}
{
}

void MppCreatureController::Visit(SavepointVisitor& visitor) 
{
    MppController::Visit(visitor);
    visitor(State);
    visitor(TargetX);
    visitor(TargetY);
    visitor(IdleTicks);
}

void MppCreatureController::Update(uint64_t ticks) 
{
    IdleTicks--;
    switch (State)
    {
    case MppCreatureControllerStateIdle: Idle(); break;
    case MppCreatureControllerStateMove: Move(); break;
    default: MppAssert(false);
    }
}

std::shared_ptr<MppCreatureEntity> MppCreatureController::GetCreature()
{
    return std::dynamic_pointer_cast<MppCreatureEntity>(Entity.lock());
}

void MppCreatureController::Idle()
{
    std::shared_ptr<MppCreatureEntity> creature = GetCreature();
    if (IdleTicks > 0)
    {
        return;
    }
    MppAssert(State == MppCreatureControllerStateIdle);
    PickRandomTarget(kMaxRandomMoveDistance, TargetX, TargetY);
    State = MppCreatureControllerStateMove;
}

void MppCreatureController::Move()
{
    std::shared_ptr<MppCreatureEntity> creature = GetCreature();
    int dx = TargetX - creature->GetX();
    int dy = TargetY - creature->GetY();
    if (!dx && !dy)
    {
        MppAssert(IdleTicks <= 0);
        if (creature->GetFleeTicks() <= 0)
        {
            IdleTicks = creature->GetIdleCooldown();
        }
        State = MppCreatureControllerStateIdle;
        return;
    }
    if (MppConsole::CVarNavigation.GetBool())
    {
        int x1 = creature->GetX() + MppTile::kSize / 2;
        int y1 = creature->GetY() + MppTile::kSize / 2;
        int x2 = TargetX + MppTile::kSize / 2;
        int y2 = TargetY + MppTile::kSize / 2;
        MppRendererDrawLine(kMppColorDebugNavigation1, x1, y1, x2, y2, MppRendererLayerDebugNavigation);
    }
    // creature->MoveSmallestAxis(dx, dy);
    creature->Move(dx, dy);
}
