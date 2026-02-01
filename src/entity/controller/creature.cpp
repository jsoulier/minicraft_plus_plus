#include <savepoint/savepoint.hpp>

#include <cstdint>

#include "../../assert.hpp"
#include "../../color.hpp"
#include "../../console.hpp"
#include "../../renderer.hpp"
#include "../../tile.hpp"
#include "../creature.hpp"
#include "creature.hpp"

MppCreatureController::MppCreatureController()
    : MppNPCController()
    , State{MppCreatureControllerStateIdle}
    , IdleTicks{}
{
}

void MppCreatureController::Visit(SavepointVisitor& visitor) 
{
    MppController::Visit(visitor);
    visitor(State);
    visitor(IdleTicks);
}

void MppCreatureController::Update(uint64_t ticks) 
{
    MppNPCController::Update(ticks);
    IdleTicks--;
    switch (State)
    {
    case MppCreatureControllerStateIdle: Idle(); break;
    case MppCreatureControllerStateMove: Move(); break;
    default: MppAssert(false);
    }
}

int MppCreatureController::GetMaxNavigateRandomDistance() const
{
    return 6;
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
    TryNavigateRandom();
    State = MppCreatureControllerStateMove;
}

void MppCreatureController::Move()
{
    std::shared_ptr<MppCreatureEntity> creature = GetCreature();
    if (IsNavigationCompleted())
    {
        MppAssert(IdleTicks <= 0);
        if (creature->GetFleeTicks() <= 0)
        {
            IdleTicks = creature->GetIdleCooldown();
        }
        State = MppCreatureControllerStateIdle;
    }
}
