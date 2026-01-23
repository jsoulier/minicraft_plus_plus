#include <array>
#include <memory>
#include <vector>

#include "entity.hpp"
#include "entity/player.hpp"
#include "gen.hpp"
#include "renderer.hpp"
#include "save.hpp"
#include "tile.hpp"
#include "world.hpp"

static constexpr int kSize = 256;

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

static std::array<Level, LevelIDCount> levels;
static LevelID level;

bool MppWorldInit()
{
    if (!MppSaveIsExisting())
    {
        for (int level = 0; level < LevelIDCount; level++)
        for (int x = 0; x < kSize; x++)
        for (int y = 0; y < kSize; y++)
        {
            levels[level].Tiles[x][y] = kGenerators[level](x, y, level);
        }
        levels[level].Entities.push_back(std::make_shared<MppPlayerEntity>());
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
}

void MppWorldUpdate(uint64_t ticks)
{
    for (std::shared_ptr<MppEntity>& entity : levels[level].Entities)
    {
        entity->Update(ticks);
    }
    for (int x = MppRendererGetTileX1(); x < MppRendererGetTileX2(); x++)
    for (int y = MppRendererGetTileY1(); y < MppRendererGetTileY2(); y++)
    {
        levels[level].Tiles[x][y].Update(x, y, ticks);
    }
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

std::vector<std::shared_ptr<MppEntity>>& MppWorldGetEntities(int level)
{
    return levels[level].Entities;
}

std::vector<std::shared_ptr<MppEntity>>& MppWorldGetEntities()
{
    return MppWorldGetEntities(level);
}

std::vector<std::shared_ptr<MppEntity>>& MppWorldGetEntities(int x, int y)
{
    // TODO: spatial queries
    return MppWorldGetEntities(level);
}

void MppWorldSetTile(const MppTile& tile, int x, int y, int level)
{
    levels[level].Tiles[x][y] = tile;
}

void MppWorldSetTile(const MppTile& tile, int x, int y)
{
    MppWorldSetTile(tile, x, y, level);
}

void MppWorldAddEntity(const std::shared_ptr<MppEntity>& entity, int level)
{
    entity->OnAddEntity();
    levels[level].Entities.push_back(entity);
}

void MppWorldAddEntity(const std::shared_ptr<MppEntity>& entity)
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
