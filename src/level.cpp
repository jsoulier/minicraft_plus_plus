#include <SDL3/SDL.h>
#include <savepoint.hpp>

#include <algorithm>
#include <memory>

#include "camera.hpp"
#include "entity.hpp"
#include "level.hpp"
#include "player_entity.hpp"
#include "renderer.hpp"
#include "world.hpp"

MppLevel::MppLevel()
    : Tiles{}
    , Entities{}
    , MinDirtyX{kWidth}
    , MinDirtyY{kWidth}
    , MaxDirtyX{0}
    , MaxDirtyY{0}
{
}

void MppLevel::Generate(MppWorld& world, int level)
{
    for (int x = 0; x < kWidth; x++)
    for (int y = 0; y < kWidth; y++)
    {
        Tiles[x][y] = Generate(x, y);
    }
}

void MppLevel::Load(MppWorld& world, Savepoint& savepoint, int level)
{
    int logs = 0;
    savepoint.Read([&](SavepointBase* base, SavepointID id)
    {
        MppEntity* entity = dynamic_cast<MppEntity*>(base);
        if (entity)
        {
            entity->ID = id;
            if (dynamic_cast<MppPlayerEntity*>(entity))
            {
                world.SetLevel(level);
            }
            AddEntity(std::shared_ptr<MppEntity>(entity));
        }
        else
        {
            SDL_Log("Bad entity");
        }
    }, level);
    savepoint.Read([&](SavepointVisitor& visitor, int x, int y)
    {
        if (IsValid(x, y))
        {
            visitor(Tiles[x][y]);
        }
        else if (logs++ < 100)
        {
            SDL_Log("Bad tile: %d, %d", x, y);
        }
    }, level);
    for (int x = 0; x < kWidth; x++)
    for (int y = 0; y < kWidth; y++)
    {
        if (Tiles[x][y].IsValid())
        {
            continue;
        }
        if (logs++ < 100)
        {
            SDL_Log("Bad tile: %d, %d", x, y);
        }
        Tiles[x][y] = Generate(x, y);
    }
}

void MppLevel::Save(Savepoint& savepoint, int level)
{
    SDL_assert(IsValid(MinDirtyX, MinDirtyY));
    SDL_assert(IsValid(MaxDirtyY, MaxDirtyY));
    for (int x = MinDirtyX; x <= MaxDirtyX; x++)
    for (int y = MinDirtyY; y <= MaxDirtyY; y++)
    {
        // TODO:
    }
    for (std::shared_ptr<MppEntity>& entity : Entities)
    {
        // TODO:
    }
    MinDirtyX = kWidth;
    MinDirtyY = kWidth;
    MaxDirtyX = 0;
    MaxDirtyY = 0;
}

void MppLevel::Update(MppWorld& world, MppRenderer& renderer, int ticks)
{
    const MppCamera& camera = renderer.GetCamera();
    SDL_assert(IsValid(camera.TileX1, camera.TileY1));
    SDL_assert(IsValid(camera.TileX2, camera.TileY2));
    for (int x = camera.TileX1; x <= camera.TileX2; x++)
    for (int y = camera.TileY1; y <= camera.TileY2; y++)
    {
        Tiles[x][y].Update(*this, renderer, x, y, ticks);
    }
    for (std::shared_ptr<MppEntity>& entity : Entities)
    {
        entity->Update(*this, renderer, ticks);
    }
    MinDirtyX = std::min(camera.TileX1, MinDirtyX);
    MinDirtyY = std::min(camera.TileY1, MinDirtyY);
    MaxDirtyX = std::max(camera.TileX2, MaxDirtyX);
    MaxDirtyY = std::max(camera.TileY2, MaxDirtyY);
}

void MppLevel::AddEntity(const std::shared_ptr<MppEntity>& entity)
{
    Entities.push_back(entity);
}

const MppTile& MppLevel::GetTile(int x, int y) const
{
    return IsValid(x, y) ? Tiles[x][y] : kMppTileInvalid;
}

bool MppLevel::IsValid(int x, int y) const
{
    return x >= 0 && y >= 0 && x < kWidth && y < kWidth;
}