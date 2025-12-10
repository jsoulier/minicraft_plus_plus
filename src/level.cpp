#include <SDL3/SDL.h>
#include <savepoint.hpp>

#include <memory>

#include "level.hpp"

MppLevel::MppLevel()
{
}

void MppLevel::Generate()
{
    for (int x = 0; x < kWidth; x++)
    for (int y = 0; y < kWidth; y++)
    {
        Tiles[x][y] = Generate(x, y);
    }
}

void MppLevel::Load(Savepoint& savepoint, int level)
{
    savepoint.Read([this](SavepointBase* base, SavepointID id)
    {
        MppEntity* entity = dynamic_cast<MppEntity*>(base);
        if (entity)
        {
            entity->ID = id;
            AddEntity(std::shared_ptr<MppEntity>(entity));
        }
        else
        {
            SDL_Log("Bad entity");
        }
    }, level);
    savepoint.Read([this](SavepointVisitor& visitor, int x, int y)
    {
        if (IsValid(x, y))
        {
            visitor(Tiles[x][y]);
        }
    }, level);
}

void MppLevel::Update(MppWorld& world, MppRenderer& renderer, float dt, float ticks)
{
}

void MppLevel::AddEntity(const std::shared_ptr<MppEntity>& entity)
{
    Entities.push_back(entity);
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