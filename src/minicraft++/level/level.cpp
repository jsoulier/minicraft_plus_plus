#include <savepoint/savepoint.hpp>

#include <algorithm>
#include <array>
#include <memory>
#include <vector>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/entity.hpp>
#include <minicraft++/level/level.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/world.hpp>

MppLevel::MppLevel()
    : SaveTileX1{0}
    , SaveTileY1{0}
    , SaveTileX2{0}
    , SaveTileY2{0}
    , Level{0}
{
}

void MppLevel::Generate()
{
    for (int x = 0; x < kSize; x++)
    for (int y = 0; y < kSize; y++)
    {
        Tiles[x][y] = GenerateTile(x, y);
    }
}

void MppLevel::Load()
{
    uint32_t tiles = 0;
    uint32_t entities = 0;
    using EntityT = std::shared_ptr<MppEntity>;
    SaveData->Handle.Read<EntityT>([&](EntityT& entity)
    {
        entity->OnCreate();
        AddEntity(entity);
        entities++;
    }, Level);
    SaveData->Handle.Read<MppTile>([&](MppTile tile, int x, int y)
    {
        Tiles[x][y] = tile;
        tiles++;
    }, Level);
    MppLog("Loaded %d tiles from level %d", tiles, Level);
    MppLog("Loaded %d entities from level %d", entities, Level);
}

void MppLevel::Update(uint64_t ticks)
{
    std::vector<std::shared_ptr<MppEntity>> entities;
    for (std::shared_ptr<MppEntity>& entity : Entities)
    {
        entity->Update(ticks);
        if (!entity->IsSpawned())
        {
            if (entity->CanBeSaved())
            {
                SaveData->Handle.Delete(*entity);
            }
        }
        else
        {
            entities.push_back(entity);
        }
    }
    Entities = std::move(entities);
    for (int x = MppRendererGetTileX1(); x <= MppRendererGetTileX2(); x++)
    for (int y = MppRendererGetTileY1(); y <= MppRendererGetTileY2(); y++)
    {
        Tiles[x][y].Update(x, y, ticks);
    }
    SaveTileX1 = std::min(SaveTileX1, MppRendererGetTileX1());
    SaveTileY1 = std::min(SaveTileY1, MppRendererGetTileY1());
    SaveTileX2 = std::max(SaveTileX2, MppRendererGetTileX2());
    SaveTileY2 = std::max(SaveTileY2, MppRendererGetTileY2());
}

void MppLevel::Render() const
{
    for (const std::shared_ptr<MppEntity>& entity : Entities)
    {
        entity->Render();
    }
    for (int x = MppRendererGetTileX1(); x <= MppRendererGetTileX2(); x++)
    for (int y = MppRendererGetTileY1(); y <= MppRendererGetTileY2(); y++)
    {
        Tiles[x][y].Render(x, y);
    }
}

void MppLevel::Save()
{
    if (SaveData->Status == SavepointStatus::New)
    {
        SaveTileX1 = 0;
        SaveTileY1 = 0;
        SaveTileX2 = kSize;
        SaveTileY2 = kSize;
    }
    uint32_t tiles = 0;
    uint32_t entities = 0;
    for (int x = SaveTileX1; x <= SaveTileX2; x++)
    for (int y = SaveTileY1; y <= SaveTileY2; y++)
    {
        SaveData->Handle.Write(Tiles[x][y], x, y, Level);
        tiles++;
    }
    for (std::shared_ptr<MppEntity>& entity : Entities)
    {
        if (entity->CanBeSaved())
        {
            SaveData->Handle.Write(entity, Level);
            entities++;
        }
    }
    MppLog("Saved %d tiles to level %d", tiles, Level);
    MppLog("Saved %d entities to level %d", entities, Level);
    SaveTileX1 = kSize;
    SaveTileY1 = kSize;
    SaveTileX2 = -1;
    SaveTileY2 = -1;
}

MppTile& MppLevel::GetTile(int x, int y)
{
    if (x >= 0 && y >= 0 && x < kSize && y < kSize)
    {
        return Tiles[x][y];
    }
    else
    {
        return kMppTileInvalid;
    }
}

void MppLevel::SetTile(const MppTile& tile, int x, int y)
{
    if (x >= 0 && y >= 0 && x < kSize && y < kSize)
    {
        Tiles[x][y] = tile;
    }
    else
    {
        MppLog("Tried to set tile at invalid coordinate: %d, %d", x, y);
    }
}

std::vector<std::shared_ptr<MppEntity>> MppLevel::GetEntities()
{
    return Entities;
}

std::vector<std::shared_ptr<MppEntity>> MppLevel::GetEntities(int x, int y)
{
    // TODO: spatial queries
    return Entities;
}

void MppLevel::AddEntity(std::shared_ptr<MppEntity>& entity)
{
    Entities.push_back(entity);
    if (!entity->IsSpawned())
    {
        entity->OnAdd();
    }
    entity->OnSetLevel(Level);
    if (entity->CanBeSaved())
    {
        SaveData->Handle.Write(entity, Level);
        SaveData->References[entity->GetID()] = entity;
    }
}

void MppLevel::RemoveEntity(std::shared_ptr<MppEntity>& entity)
{
    int count = std::erase_if(Entities, [&entity](const std::shared_ptr<MppEntity>& other)
    {
        return entity == other;
    });
    MppAssert(count == 1);
}

void MppLevel::SetSaveData(const std::shared_ptr<MppLevelSaveData>& state)
{
    SaveData = state;
}

void MppLevel::SetLevel(int level)
{
    Level = level;
}

int MppLevel::GetLightColor() const
{
    return 555;
}
