#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/console.hpp>
#include <minicraft++/entity/controller/npc.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/random.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/util.hpp>
#include <minicraft++/world.hpp>

void MppNPCController::Visit(SavepointVisitor& visitor)
{
    MppController::Visit(visitor);
    visitor(Points);
    visitor(PointIndex);
}

void MppNPCController::Update(uint64_t ticks)
{
    MppController::Update(ticks);
    if (IsNavigationCompleted())
    {
        return;
    }
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    int dx = Points[PointIndex].first - entity->GetX();
    int dy = Points[PointIndex].second - entity->GetY();
    if (dx == 0 && dy == 0)
    {
        PointIndex++;
        if (PointIndex >= Points.size())
        {
            CancelNavigation();
            return;
        }
    }
    dx = Points[PointIndex].first - entity->GetX();
    dy = Points[PointIndex].second - entity->GetY();
    dx = MppNormalize(dx);
    dy = MppNormalize(dy);
    // Apply speed if we're not going to overshoot
    bool useSpeedX = std::abs(dx) >= entity->GetSpeed();
    bool useSpeedY = std::abs(dy) >= entity->GetSpeed();
    entity->Push(dx, 0, useSpeedX);
    entity->Push(0, dy, useSpeedX);
    if (MppConsole::CVarNavigation.GetBool())
    {
        int x1 = entity->GetX() + MppTile::kSize / 2;
        int y1 = entity->GetY() + MppTile::kSize / 2;
        int x2 = Points[PointIndex].first + MppTile::kSize / 2;
        int y2 = Points[PointIndex].second + MppTile::kSize / 2;
        MppRendererDrawLine(kMppColorDebugNavigation, x1, y1, x2, y2, MppRendererLayerDebugNavigation);
        for (int i = PointIndex; i < Points.size() - 1; i++)
        {
            int x1 = Points[i + 0].first + MppTile::kSize / 2;
            int y1 = Points[i + 0].second + MppTile::kSize / 2;
            int x2 = Points[i + 1].first + MppTile::kSize / 2;
            int y2 = Points[i + 1].second + MppTile::kSize / 2;
            MppRendererDrawLine(kMppColorDebugNavigation, x1, y1, x2, y2, MppRendererLayerDebugNavigation);
        }
    }
}

void MppNPCController::OnSetLevel(int level)
{
    MppController::OnSetLevel(level);
    CancelNavigation();
}

bool MppNPCController::TryNavigateRandom()
{
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    MppAssert(PointIndex == 0);
    MppAssert(Points.empty());
    int dx = 0;
    int dy = 0;
    switch (MppRandomGetInt(0, 3))
    {
    case 0: dx = -1; break;
    case 1: dx = 1; break;
    case 2: dy = -1; break;
    case 3: dy = 1; break;
    }
    int distance = MppRandomGetInt(1, GetMaxNavigateRandomDistance());
    int tx = MppWorldGetTileIndex(entity->GetX());
    int ty = MppWorldGetTileIndex(entity->GetY());
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
    Points.emplace_back(tx * MppTile::kSize, ty * MppTile::kSize);
    return true;
}

bool MppNPCController::TryNavigateLine(const std::shared_ptr<MppEntity>& target)
{
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    MppAssert(PointIndex == 0);
    MppAssert(Points.empty());
    int x1 = entity->GetX() + MppTile::kSize / 2;
    int y1 = entity->GetY() + MppTile::kSize / 2;
    int x2 = target->GetX() + MppTile::kSize / 2;
    int y2 = target->GetY() + MppTile::kSize / 2;
    int tx = MppWorldGetTileIndex(x1);
    int ty = MppWorldGetTileIndex(y1);
    int tx2 = MppWorldGetTileIndex(x2);
    int ty2 = MppWorldGetTileIndex(y2);
    int dx = std::abs(tx2 - tx);
    int dy = std::abs(ty2 - ty);
    int sx = (tx < tx2) ? 1 : -1;
    int sy = (ty < ty2) ? 1 : -1;
    int error = dx - dy;
    while (true)
    {
        const MppTile& tile = MppWorldGetTile(tx, ty);
        if (tile.GetPhysicsType() == MppTilePhysicsTypeWall)
        {
            Points.clear();
            return false;
        }
        int x = tx * MppTile::kSize;
        int y = ty * MppTile::kSize;
        Points.emplace_back(x, y);
        if (tx == tx2 && ty == ty2)
        {
            break;
        }
        int e2 = error * 2;
        if (e2 > -dy)
        {
            error -= dy;
            tx += sx;
        }
        else if (e2 < dx)
        {
            error += dx;
            ty += sy;
        }
    }
    return true;
}

void MppNPCController::CancelNavigation()
{
    Points.clear();
    PointIndex = 0;
}

bool MppNPCController::IsNavigationCompleted() const
{
    return Points.empty();
}

int MppNPCController::GetMaxNavigateRandomDistance() const
{
    return 1;
}
