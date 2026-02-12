#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/console.hpp>
#include <minicraft++/entity/controller/npc.hpp>
#include <minicraft++/entity/mob/mob.hpp>
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
    // TODO: there's a bug where speeds > 1 mean they'll keep skipping past their target
    int speed = entity->GetSpeed();
    if (std::abs(dx) < speed && std::abs(dy) < speed)
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
    entity->Push(dx, dy);
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

bool MppNPCController::TryNavigateRandom()
{
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    MppAssert(PointIndex == 0);
    MppAssert(Points.empty());
    int dx = 0;
    int dy = 0;
    switch (MppGetRandomInt(0, 3))
    {
    case 0: dx = -1; break;
    case 1: dx = 1; break;
    case 2: dy = -1; break;
    case 3: dy = 1; break;
    }
    int distance = MppGetRandomInt(1, GetMaxNavigateRandomDistance());
    int tx = entity->GetX() / MppTile::kSize;
    int ty = entity->GetY() / MppTile::kSize;
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
    int tx = x1 / MppTile::kSize;
    int ty = y1 / MppTile::kSize;
    int tx2 = x2 / MppTile::kSize;
    int ty2 = y2 / MppTile::kSize;
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
