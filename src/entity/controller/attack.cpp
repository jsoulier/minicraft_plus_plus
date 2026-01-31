#include <SDL3/SDL.h>
#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "../../assert.hpp"
#include "../../color.hpp"
#include "../../console.hpp"
#include "../../log.hpp"
#include "../../renderer.hpp"
#include "../../tile.hpp"
#include "../../world.hpp"
#include "../mob.hpp"
#include "attack.hpp"

static constexpr int kMaxRandomMoveDistance = 2;

MppAttackController::MppAttackController()
    : MppController()
    , Target{}
    , State{}
    , Points{}
    , PointIndex{0}
{
}

void MppAttackController::Visit(SavepointVisitor& visitor)
{
    MppController::Visit(visitor);
    visitor(Target);
    visitor(State);
    visitor(Points);
    visitor(PointIndex);
}

void MppAttackController::Update(uint64_t ticks)
{
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    Target.Update();
    switch (State)
    {
    case MppAttackControllerStateIdle: Idle(); break;
    case MppAttackControllerStateMove: Move(); break;
    default: MppAssert(false);
    }
}

void MppAttackController::Idle()
{
    // TODO: cooldown on searching through entities
    MppAssert(!Target.IsValid());
    MppAssert(Points.empty());
    MppAssert(PointIndex == 0);
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
        std::vector<std::pair<int, int>> points = entity->Raycast(mob);
        if (points.empty())
        {
            continue;
        }
        // TODO: compare teams
        Points = std::move(points);
        Target.SetEntity(mob);
    }
    MppAssert(PointIndex == 0);
    if (!Target.IsValid() /* && we're done idling */)
    {
        MppAssert(Points.empty());
        std::pair<int, int> point = std::make_pair(0, 0);
        PickRandomTarget(kMaxRandomMoveDistance, point.first, point.second);
        Points.push_back(point);
    }
    else
    {
        MppAssert(!Points.empty());
        int targetX = 0;
        int targetY = 0;
        std::shared_ptr<MppMobEntity> mob = std::dynamic_pointer_cast<MppMobEntity>(Target.GetEntity());
        GetAttackPosition(mob, targetX, targetY);
        // TODO: Don't have support for attack position yet.
        // To add it, we need to do another raycast from current to target. If it's reachable,
        // override the points and use that. Otherwise, push the targetX and targetY as another point
        // to the end (which is guaranteed to be reachable, just not optimal).
        MppAssert(Target.GetEntity()->GetX() == targetX);
        MppAssert(Target.GetEntity()->GetY() == targetY);
    }
    MppAssert(!Points.empty());
    State = MppAttackControllerStateMove;
}

void MppAttackController::Move()
{
    MppAssert(!Points.empty());
    std::shared_ptr<MppMobEntity> target = std::dynamic_pointer_cast<MppMobEntity>(Target.GetEntity());
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    if (target)
    {
        int range = entity->GetActionRange();
        int distance = entity->GetDistance(target);
        if (distance <= range)
        {
            // TODO: attack
            // need to figure out how I want the OnAction to work. maybe entity->Attack()
            // and target->OnAction(entity) or something?
        }
    }
    int dx = Points[PointIndex].first - entity->GetX();
    int dy = Points[PointIndex].second - entity->GetY();
    if (!dx && !dy)
    {
        PointIndex++;
        if (PointIndex >= Points.size())
        {
            State = MppAttackControllerStateIdle;
            Points.clear();
            PointIndex = 0;
            Target.SetEntity(nullptr);
            return;
        }
    }
    dx = Points[PointIndex].first - entity->GetX();
    dy = Points[PointIndex].second - entity->GetY();
    entity->Move(dx, dy);
    // if (dx && dy)
    // {

    // }
    // else
    // {

    // }
    // entity->MoveSmallestAxis(dx, dy);
    if (MppConsole::CVarNavigation.GetBool())
    {
        static constexpr int kColors[] = {kMppColorDebugNavigation1, kMppColorDebugNavigation2};
        int color = 0;
        int x1 = entity->GetX() + MppTile::kSize / 2;
        int y1 = entity->GetY() + MppTile::kSize / 2;
        int x2 = Points[PointIndex].first + MppTile::kSize / 2;
        int y2 = Points[PointIndex].second + MppTile::kSize / 2;
        MppRendererDrawLine(kColors[color], x1, y1, x2, y2, MppRendererLayerDebugNavigation);
        for (int i = PointIndex; i < Points.size() - 1; i++)
        {
            int x1 = Points[i + 0].first + MppTile::kSize / 2;
            int y1 = Points[i + 0].second + MppTile::kSize / 2;
            int x2 = Points[i + 1].first + MppTile::kSize / 2;
            int y2 = Points[i + 1].second + MppTile::kSize / 2;
            color = (color + 1) % SDL_arraysize(kColors);
            MppRendererDrawLine(kColors[color], x1, y1, x2, y2, MppRendererLayerDebugNavigation);
        }
    }
}
