#include <SDL3/SDL.h>
#include <savepoint.hpp>

#include <memory>

#include "level.hpp"

MppLevel::MppLevel()
{
}

void MppLevel::Generate()
{
}

void MppLevel::Load(Savepoint& savepoint, int level)
{
    savepoint.Read([&](SavepointBase* base, SavepointID id)
    {
        MppEntity* entity = dynamic_cast<MppEntity*>(base);
        if (!entity)
        {
            SDL_Log("Bad entity");
            return;
        }
        entity->ID = id;
        AddEntity(std::shared_ptr<MppEntity>(entity));
    }, level);
    savepoint.Read([&](SavepointVisitor& visitor, int x, int y)
    {
        MppTile tile;
        visitor(tile);
        SetTile(tile, x, y);
    }, level);
}

void MppLevel::AddEntity(const std::shared_ptr<MppEntity>& entity)
{
    Entities.push_back(entity);
}

void MppLevel::SetTile(MppTile tile, int x, int y)
{
    if (IsValid(x, y))
    {
        Tiles[x][y] = tile;
    }
    else
    {
        SDL_Log("Bad tile position: %d, %d", x, y);
    }
}

const MppTile& MppLevel::GetTile(int x, int y) const
{
    if (IsValid(x, y))
    {
        return Tiles[x][y];
    }
    else
    {
        return kMppTileInvalid;
    }
}

bool MppLevel::IsValid(int x, int y) const
{
    return x >= 0 && y >= 0 && x < kWidth && y < kWidth;
}