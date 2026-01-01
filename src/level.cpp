#include <SDL3/SDL.h>
#include <savepoint/savepoint.hpp>

#include <algorithm>
#include <memory>
#include <utility>

#include "camera.hpp"
#include "entity.hpp"
#include "level.hpp"
#include "player_entity.hpp"
#include "renderer.hpp"
#include "world.hpp"

MppLevel::MppLevel()
    : Tiles{}
    , Entities{}
    , MinDirtyX{kWidth - 1}
    , MinDirtyY{kWidth - 1}
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
    savepoint.Read<std::shared_ptr<MppEntity>>([&](std::shared_ptr<MppEntity>& entity, SavepointID id)
    {
        entity->ID = id;
        if (dynamic_cast<MppPlayerEntity*>(entity.get()))
        {
            world.SetLevel(level);
        }
        Add(std::shared_ptr<MppEntity>(entity));
    }, level);
    savepoint.Read<MppTile>([&](MppTile& tile, int x, int y)
    {
        if (IsValid(x, y))
        {
            Tiles[x][y] = tile;
        }
        else if (logs++ < 10)
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
        if (logs++ < 10)
        {
            SDL_Log("Bad tile: %d, %d", x, y);
        }
        Tiles[x][y] = Generate(x, y);
    }
}

void MppLevel::Save(Savepoint& savepoint, int level, bool all)
{
    SDL_assert(IsValid(MinDirtyX, MinDirtyY));
    SDL_assert(IsValid(MaxDirtyY, MaxDirtyY));
    int x1 = 0;
    int y1 = 0;
    int x2 = kWidth - 1;
    int y2 = kWidth - 1;
    if (!all)
    {
        x1 = MinDirtyX;
        x2 = MaxDirtyX;
        y1 = MinDirtyY;
        y2 = MaxDirtyY;
    }
    for (int x = x1; x <= x2; x++)
    for (int y = y1; y <= y2; y++)
    {
        savepoint.Write(Tiles[x][y], x, y, level);
    }
    for (std::shared_ptr<MppEntity>& entity : Entities)
    {
        savepoint.Write(entity, entity->ID, level);
    }
    MinDirtyX = kWidth - 1;
    MinDirtyY = kWidth - 1;
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
    for (std::shared_ptr<MppEntity>& entity : std::exchange(AddedEntities, {}))
    {
        entity->Update(*this, renderer, ticks);
        Entities.push_back(entity);
    }
    // TODO: may cause issues later with repeatedly adding entities
    SDL_assert(AddedEntities.empty());
    MinDirtyX = std::min(camera.TileX1, MinDirtyX);
    MinDirtyY = std::min(camera.TileY1, MinDirtyY);
    MaxDirtyX = std::max(camera.TileX2, MaxDirtyX);
    MaxDirtyY = std::max(camera.TileY2, MaxDirtyY);
}

void MppLevel::Add(const std::shared_ptr<MppEntity>& entity)
{
    AddedEntities.push_back(entity);
}

const MppTile& MppLevel::GetTile(int x, int y) const
{
    return IsValid(x, y) ? Tiles[x][y] : kMppTileInvalid;
}

bool MppLevel::IsValid(int x, int y) const
{
    return x >= 0 && y >= 0 && x < kWidth && y < kWidth;
}