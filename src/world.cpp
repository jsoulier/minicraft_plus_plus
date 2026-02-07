#include <SDL3/SDL.h>
#include <savepoint/savepoint.hpp>

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "assert.hpp"
#include "entity.hpp"
#include "entity/player.hpp"
#include "gen.hpp"
#include "log.hpp"
#include "renderer.hpp"
#include "tile.hpp"
#include "version.hpp"
#include "world.hpp"

static constexpr int kSize = 256;

struct SaveHeader
{
    uint64_t Ticks;

    SaveHeader()
        : Ticks{}
    {
    }

    void Visit(SavepointVisitor& visitor)
    {
        visitor(Ticks);
    }
};

enum LevelID
{
    LevelIDSurface,
    LevelIDUnderground1,
    LevelIDUnderground2,
    LevelIDUnderground3,
    LevelIDCount,
};

struct Level
{
    std::array<std::array<MppTile, kSize>, kSize> Tiles;
    std::vector<std::shared_ptr<MppEntity>> Entities;
};

static constexpr std::array<MppGenFunction, LevelIDCount> kGenerators =
{
    MppGenSurface,
    MppGenUnderground,
    MppGenUnderground,
    MppGenUnderground,
};

static const std::filesystem::path kPrefPath = SDL_GetPrefPath(nullptr, "minicraft++");
static const std::filesystem::path kSavePath = kPrefPath / "minicraft++.savepoint";
static constexpr uint64_t kSaveRate = 1000;

static std::array<Level, LevelIDCount> levels;
static std::unordered_map<SavepointID, std::weak_ptr<MppEntity>> references;
static LevelID level;
static Savepoint savepoint;
static SavepointStatus savepointStatus;
static SaveHeader saveHeader;
static uint64_t saveTicks;
static int saveTileX1;
static int saveTileY1;
static int saveTileX2;
static int saveTileY2;

bool MppWorldInit()
{
    SavepointSetLogFunction([](const std::string_view& string)
    {
        MppLog("Savepoint: %s", string.data());
    });
    savepointStatus = savepoint.Open(SavepointDriver::SQLite3, kSavePath.string(), kMppVersion);
    switch (savepointStatus)
    {
    case SavepointStatus::Failed:
        MppLog("Failed to open savepoint");
        savepoint.Open(SavepointDriver::Null, kSavePath.string(), kMppVersion);
        break;
    case SavepointStatus::Existing:
        {
            MppLog("Read an existing save");
            savepoint.Read(saveHeader);
            MppLog("Save Path: %s", kSavePath.string().data());
            uint32_t tiles = 0;
            uint32_t entities = 0;
            for (int level : savepoint.GetLevels())
            {
                using EntityT = std::shared_ptr<MppEntity>;
                savepoint.Read<EntityT>([&](EntityT& entity)
                {
                    MppWorldAddEntity(entity, level);
                    entities++;
                }, level);
                savepoint.Read<MppTile>([&](MppTile tile, int x, int y)
                {
                    MppWorldSetTile(tile, x, y, level);
                    tiles++;
                }, level);
            }
            MppLog("Loaded %d tiles", tiles);
            MppLog("Loaded %d entities", entities);
        }
        break;
    case SavepointStatus::New:
        MppLog("Created a new save");
        savepoint.Write(saveHeader);
        break;
    }
    if (savepointStatus != SavepointStatus::Existing)
    {
        for (int level = 0; level < LevelIDCount; level++)
        for (int x = 0; x < kSize; x++)
        for (int y = 0; y < kSize; y++)
        {
            levels[level].Tiles[x][y] = kGenerators[level](x, y, level);
        }
        std::shared_ptr<MppEntity> player = std::make_shared<MppPlayerEntity>();
        MppWorldAddEntity(player);
        level = LevelIDSurface;
    }
    return true;
}

void MppWorldQuit()
{
    for (int level = 0; level < LevelIDCount; level++)
    {
        levels[level].Entities.clear();
    }
    references.clear();
}

void MppWorldUpdate(uint64_t ticks)
{
    std::vector<std::shared_ptr<MppEntity>> entities;
    for (std::shared_ptr<MppEntity>& entity : levels[level].Entities)
    {
        entity->Update(ticks);
        if (entity->IsKilled())
        {
            if (entity->CanSave())
            {
                savepoint.Delete(*entity);
            }
        }
        else
        {
            entities.push_back(entity);
        }
    }
    levels[level].Entities = std::move(entities);
    for (int x = MppRendererGetTileX1(); x < MppRendererGetTileX2(); x++)
    for (int y = MppRendererGetTileY1(); y < MppRendererGetTileY2(); y++)
    {
        levels[level].Tiles[x][y].Update(x, y, ticks);
    }
}

void MppWorldSave(uint64_t inTicks, bool force)
{
    if (savepointStatus == SavepointStatus::Failed)
    {
        return;
    }
    saveTileX1 = std::min(saveTileX1, MppRendererGetTileX1());
    saveTileY1 = std::min(saveTileY1, MppRendererGetTileY1());
    saveTileX2 = std::max(saveTileX2, MppRendererGetTileX2());
    saveTileY2 = std::max(saveTileY2, MppRendererGetTileY2());
    if (saveTicks > inTicks && !force)
    {
        return;
    }
    saveHeader.Ticks = inTicks;
    savepoint.Write(saveHeader);
    int level1;
    int level2;
    if (savepointStatus == SavepointStatus::New)
    {
        level1 = 0;
        level2 = MppWorldGetLevelCount();
        saveTileX1 = 0;
        saveTileY1 = 0;
        saveTileX2 = MppWorldGetSize();
        saveTileY2 = MppWorldGetSize();
        savepointStatus = SavepointStatus::Existing;
    }
    else
    {
        level1 = MppWorldGetLevel();
        level2 = level1 + 1;
    }
    uint32_t tiles = 0;
    uint32_t entities = 0;
    for (int level = level1; level < level2; level++)
    {
        for (int x = saveTileX1; x < saveTileX2; x++)
        for (int y = saveTileY1; y < saveTileY2; y++)
        {
            savepoint.Write(MppWorldGetTile(x, y, level), x, y, level);
            tiles++;
        }
        for (std::shared_ptr<MppEntity>& entity : MppWorldGetEntities(level))
        {
            if (entity->CanSave())
            {
                savepoint.Write(entity, level);
                entities++;
            }
        }
    }
    savepoint.Save();
    saveTicks = inTicks + kSaveRate;
    MppLog("Saved %d tiles", tiles);
    MppLog("Saved %d entities", entities);
    saveTileX1 = MppWorldGetSize();
    saveTileY1 = MppWorldGetSize();
    saveTileX2 = -1;
    saveTileY2 = -1;
}

void MppWorldRender()
{
    for (const std::shared_ptr<MppEntity>& entity : levels[level].Entities)
    {
        entity->Render();
    }
    for (int x = MppRendererGetTileX1(); x < MppRendererGetTileX2(); x++)
    for (int y = MppRendererGetTileY1(); y < MppRendererGetTileY2(); y++)
    {
        levels[level].Tiles[x][y].Render(x, y);
    }
}

MppTile& MppWorldGetTile(int x, int y, int level)
{
    if (x >= 0 && y >= 0 && x < kSize && y < kSize)
    {
        return levels[level].Tiles[x][y];
    }
    else
    {
        return kMppTileInvalid;
    }
}

MppTile& MppWorldGetTile(int x, int y)
{
    return MppWorldGetTile(x, y, level);
}

std::vector<std::shared_ptr<MppEntity>> MppWorldGetEntities(int level)
{
    return levels[level].Entities;
}

std::vector<std::shared_ptr<MppEntity>> MppWorldGetEntities()
{
    return MppWorldGetEntities(level);
}

std::vector<std::shared_ptr<MppEntity>> MppWorldGetEntities(int x, int y)
{
    // TODO: spatial queries
    return MppWorldGetEntities(level);
}

std::shared_ptr<MppEntity> MppWorldGetEntity(SavepointID id)
{
    auto it = references.find(id);
    if (it != references.end())
    {
        if (it->second.expired())
        {
            references.erase(it);
        }
        else
        {
            return it->second.lock();
        }
    }
    return nullptr;
}

void MppWorldSetTile(const MppTile& tile, int x, int y, int level)
{
    levels[level].Tiles[x][y] = tile;
}

void MppWorldSetTile(const MppTile& tile, int x, int y)
{
    MppWorldSetTile(tile, x, y, level);
}

void MppWorldAddEntity(std::shared_ptr<MppEntity>& entity, int level)
{
    entity->OnAddEntity();
    levels[level].Entities.push_back(entity);
    if (entity->CanSave())
    {
        savepoint.Write(entity, level);
    }
    references[entity->GetID()] = entity;
}

void MppWorldAddEntity(std::shared_ptr<MppEntity>& entity)
{
    MppWorldAddEntity(entity, level);
}

int MppWorldGetLevel()
{
    return level;
}

int MppWorldGetLevelCount()
{
    return LevelIDCount;
}

int MppWorldGetSize()
{
    return kSize;
}

uint64_t MppWorldGetTicks()
{
    return saveHeader.Ticks;
}
