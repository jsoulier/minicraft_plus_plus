#include <SDL3/SDL.h>
#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/console.hpp>
#include <minicraft++/entity/controller/melee.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/world.hpp>

MppMeleeController::MppMeleeController()
    : MppNPCController()
    , Target{}
    , State{}
{
}

void MppMeleeController::Visit(SavepointVisitor& visitor)
{
    MppNPCController::Visit(visitor);
    visitor(Target);
    visitor(State);
}

void MppMeleeController::Update(uint64_t ticks)
{
    MppNPCController::Update(ticks);
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    Target.Update();
    switch (State)
    {
    case MppMeleeControllerStateIdle: Idle(); break;
    case MppMeleeControllerStateMove: Move(); break;
    default: MppAssert(false);
    }
}

int MppMeleeController::GetMaxNavigateRandomDistance() const
{
    return 2;
}

void MppMeleeController::Idle()
{
    // TODO: cooldown on searching through entities
    MppAssert(!Target.IsValid());
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    for (std::shared_ptr<MppEntity>& other : MppWorldGetEntities(entity->GetX(), entity->GetY()))
    {
        std::shared_ptr<MppMobEntity> mob = std::dynamic_pointer_cast<MppMobEntity>(other);
        if (!mob)
        {
            continue;
        }
        if (!entity->IsInFov(mob))
        {
            continue;
        }
        if (!TryNavigateLine(mob))
        {
            continue;
        }
        // TODO: compare teams
        Target = mob->GetReference();
        break;
    }
    if (!Target.IsValid() /* && we're done idling */)
    {
        TryNavigateRandom();
    }
    State = MppMeleeControllerStateMove;
}

void MppMeleeController::Move()
{
    std::shared_ptr<MppMobEntity> target = std::dynamic_pointer_cast<MppMobEntity>(Target.GetEntity());
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    if (target)
    {
        int range = entity->GetActionRange();
        int distance = entity->GetDistance(target);
        if (distance <= range)
        {
            // TODO: attack
            // need to figure out how I want the OnAction to work. maybe entity->Melee()
            // and target->OnAction(entity) or something?
        }
    }
    if (IsNavigationCompleted())
    {
        State = MppMeleeControllerStateIdle;
        Target = MppEntityReference{};
    }
}
