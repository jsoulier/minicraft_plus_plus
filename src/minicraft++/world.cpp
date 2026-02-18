#include <SDL3/SDL.h>
#include <savepoint/savepoint.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/entity.hpp>
#include <minicraft++/level/level.hpp>
#include <minicraft++/level/surface.hpp>
#include <minicraft++/level/underground.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/version.hpp>
#include <minicraft++/world.hpp>

enum LevelID : uint8_t
{
    LevelIDSurface,
    LevelIDUnderground1,
    LevelIDUnderground2,
    LevelIDUnderground3,
    LevelIDCount,
};

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

static const std::filesystem::path kPrefPath = SDL_GetPrefPath(nullptr, "minicraft++");
static const std::filesystem::path kSavePath = kPrefPath / "minicraft++.savepoint";
static constexpr uint64_t kSaveRate = 1000;

static std::array<std::shared_ptr<MppLevel>, LevelIDCount> levels;
static std::vector<std::pair<std::shared_ptr<MppEntity>, int>> requests;
static std::shared_ptr<MppLevelSaveData> saveData;
static SaveHeader saveHeader;
static uint64_t saveTicks;
static LevelID currLevel;
static LevelID nextLevel;

bool MppWorldInit()
{
    saveData = std::make_shared<MppLevelSaveData>();
    levels[LevelIDSurface] = std::make_shared<MppSurfaceLevel>();
    levels[LevelIDUnderground1] = std::make_shared<MppUndergroundLevel>();
    levels[LevelIDUnderground2] = std::make_shared<MppUndergroundLevel>();
    levels[LevelIDUnderground3] = std::make_shared<MppUndergroundLevel>();
    for (int i = 0; i < LevelIDCount; i++)
    {
        levels[i]->SetSaveData(saveData);
        levels[i]->SetLevel(i);
    }
    SavepointSetLogFunction([](const std::string_view& string)
    {
        MppLog("Savepoint: %s", string.data());
    });
    saveData->Status = saveData->Handle.Open(SavepointDriver::SQLite3, kSavePath.string(), kMppVersion);
    switch (saveData->Status)
    {
    case SavepointStatus::Failed:
        MppLog("Failed to open savepoint");
        saveData->Handle.Open(SavepointDriver::Null, kSavePath.string(), kMppVersion);
        break;
    case SavepointStatus::Existing:
        MppLog("Read an existing save");
        saveData->Handle.Read(saveHeader);
        MppLog("Save Path: %s", kSavePath.string().data());
        for (int level : saveData->Handle.GetLevels())
        {
            levels[level]->Load();
        }
        break;
    case SavepointStatus::New:
        MppLog("Created a new save");
        saveData->Handle.Write(saveHeader);
        break;
    }
    if (saveData->Status != SavepointStatus::Existing)
    {
        for (int i = 0; i < LevelIDCount; i++)
        {
            levels[i]->Generate();
        }
    }
    return true;
}

void MppWorldQuit()
{
    std::fill(levels.begin(), levels.end(), nullptr);
    saveData = nullptr;
}

void MppWorldUpdate(uint64_t ticks)
{
    if (nextLevel != currLevel)
    {
        MppWorldSave(ticks, true);
        currLevel = nextLevel;
    }
    std::unordered_set<std::shared_ptr<MppEntity>> entities;
    levels[currLevel]->Update(ticks);
    for (auto& [entity, levelRequest] : requests)
    {
        if (entity->IsSpawned())
        {
            levels[currLevel]->RemoveEntity(entity);
        }
        levels[levelRequest]->AddEntity(entity);
        entity->OnSetLevel(levelRequest);
    }
    requests.clear();
}

void MppWorldSave(uint64_t inTicks, bool force)
{
    if (saveData->Status == SavepointStatus::Failed)
    {
        return;
    }
    if (saveTicks > inTicks && !force)
    {
        return;
    }
    saveHeader.Ticks = inTicks;
    saveData->Handle.Write(saveHeader);
    int level1;
    int level2;
    if (saveData->Status == SavepointStatus::New)
    {
        level1 = 0;
        level2 = MppWorldGetLevelCount();
    }
    else
    {
        level1 = MppWorldGetLevel();
        level2 = level1 + 1;
    }
    for (int level = level1; level < level2; level++)
    {
        levels[level]->Save();
    }
    saveData->Status = SavepointStatus::Existing;
    saveData->Handle.Save();
    saveTicks = inTicks + kSaveRate;
}

void MppWorldRender()
{
    levels[currLevel]->Render();
}

MppTile& MppWorldGetTile(int x, int y, int level)
{
    return levels[level]->GetTile(x, y);
}

MppTile& MppWorldGetTile(int x, int y)
{
    return MppWorldGetTile(x, y, currLevel);
}

std::vector<std::shared_ptr<MppEntity>> MppWorldGetEntities(int level)
{
    return levels[level]->GetEntities();
}

std::vector<std::shared_ptr<MppEntity>> MppWorldGetEntities()
{
    return MppWorldGetEntities(currLevel);
}

std::vector<std::shared_ptr<MppEntity>> MppWorldGetEntities(int x, int y)
{
    return levels[currLevel]->GetEntities(x, y);
}

std::shared_ptr<MppEntity> MppWorldGetEntity(SavepointID id)
{
    auto it = saveData->References.find(id);
    if (it != saveData->References.end())
    {
        if (it->second.expired())
        {
            saveData->References.erase(it);
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
    levels[level]->SetTile(tile, x, y);
}

void MppWorldSetTile(const MppTile& tile, int x, int y)
{
    MppWorldSetTile(tile, x, y, currLevel);
}

void MppWorldAddEntity(std::shared_ptr<MppEntity>& entity, int level)
{
    requests.emplace_back(entity, level);
}

void MppWorldAddEntity(std::shared_ptr<MppEntity>& entity)
{
    MppWorldAddEntity(entity, currLevel);
}

void MppWorldSetLevel(const std::shared_ptr<MppEntity>& entity, int level)
{
    requests.emplace_back(entity, level);
}

void MppWorldSetLevel(int level)
{
    nextLevel = LevelID(level);
}

int MppWorldGetLevel()
{
    return currLevel;
}

int MppWorldGetLevelCount()
{
    return LevelIDCount;
}

int MppWorldGetSize()
{
    return MppLevel::kSize;
}

int MppWorldGetLightColor()
{
    return levels[currLevel]->GetLightColor();
}

uint64_t MppWorldGetTicks()
{
    return saveHeader.Ticks;
}

int MppWorldGetTileIndex(float position)
{
    return std::floor(position / MppTile::kSize);
}
