#include <SDL3/SDL.h>
#include <savepoint/savepoint.hpp>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string_view>

#include "entity.hpp"
#include "log.hpp"
#include "renderer.hpp"
#include "save.hpp"
#include "tile.hpp"
#include "version.hpp"
#include "world.hpp"

struct Header
{
    SavepointTime CreateTime;
    SavepointTime LastModifyTime;
    uint64_t Ticks;

    Header()
        : CreateTime{}
        , LastModifyTime{}
        , Ticks{}
    {
    }

    void Visit(SavepointVisitor& visitor)
    {
        visitor(CreateTime);
        visitor(LastModifyTime);
        visitor(Ticks);
    }
};

static const std::filesystem::path kPrefPath = SDL_GetPrefPath(nullptr, "minicraft++");
static const std::filesystem::path kSavePath = kPrefPath / "minicraft++.savepoint";
static constexpr uint64_t kTicks = 10000;

static Savepoint savepoint;
static SavepointStatus status;
static Header header;
static uint64_t ticks;
static int tileX1;
static int tileY1;
static int tileX2;
static int tileY2;

bool MppSaveInit()
{
    SavepointSetLogFunction([](const std::string_view& string)
    {
        MppLog("Savepoint: %s", string.data());
    });
    status = savepoint.Open(SavepointDriver::SQLite3, kSavePath.string(), kMppVersion);
    switch (status)
    {
    case SavepointStatus::Failed:
        MppLog("Failed to open savepoint");
        savepoint.Open(SavepointDriver::Null, kSavePath.string(), kMppVersion);
        return false;
    case SavepointStatus::Existing:
        MppLog("Read an existing save");
        savepoint.Read(header);
        break;
    case SavepointStatus::New:
        MppLog("Created a new save");
        savepoint.Write(header);
        break;
    }
    MppLog("Save Path: %s", kSavePath.string().data());
    MppLog("Create Time: %s", header.CreateTime.GetString().data());
    MppLog("Last Modify Time: %s", header.LastModifyTime.GetString().data());
    uint32_t tiles = 0;
    uint32_t entities = 0;
    for (int level : savepoint.GetLevels())
    {
        using EntityT = std::shared_ptr<MppEntity>;
        savepoint.Read<EntityT>([&](EntityT& entity, SavepointID id)
        {
            entity->ID = id;
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
    return true;
}

void MppSaveQuit()
{
    savepoint.Close();
}

uint64_t MppSaveGetTicks()
{
    return header.Ticks;
}

void MppSaveUpdate(uint64_t inTicks, bool force)
{
    if (status == SavepointStatus::Failed)
    {
        return;
    }
    tileX1 = std::min(tileX1, MppRendererGetTileX1());
    tileY1 = std::min(tileY1, MppRendererGetTileY1());
    tileX2 = std::max(tileX2, MppRendererGetTileX2());
    tileY2 = std::max(tileY2, MppRendererGetTileY2());
    if (ticks > inTicks && !force)
    {
        return;
    }
    header.LastModifyTime = SavepointTime{};
    header.Ticks = inTicks;
    savepoint.Write(header);
    int level1;
    int level2;
    if (status == SavepointStatus::New)
    {
        level1 = 0;
        level2 = MppWorldGetLevelCount();
        tileX1 = 0;
        tileY1 = 0;
        tileX2 = MppWorldGetSize();
        tileY2 = MppWorldGetSize();
        status = SavepointStatus::Existing;
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
        for (int x = tileX1; x < tileX2; x++)
        for (int y = tileY1; y < tileY2; y++)
        {
            savepoint.Write(MppWorldGetTile(x, y, level), x, y, level);
            tiles++;
        }
        for (std::shared_ptr<MppEntity>& entity : MppWorldGetEntities(level))
        {
            savepoint.Write(entity, entity->ID, level);
            entities++;
        }
    }
    savepoint.Save();
    ticks = ticks + kTicks;
    MppLog("Saved at: %s", header.LastModifyTime.GetString().data());
    MppLog("Saved %d tiles", tiles);
    MppLog("Saved %d entities", entities);
    tileX1 = MppWorldGetSize();
    tileY1 = MppWorldGetSize();
    tileX2 = -1;
    tileY2 = -1;
}
