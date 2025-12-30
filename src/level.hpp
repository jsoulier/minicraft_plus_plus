#pragma once

#include <savepoint/fwd.hpp>

#include <array>
#include <memory>
#include <vector>

#include "entity.hpp"
#include "tile.hpp"

class MppRenderer;
class MppWorld;

class MppLevel
{
public:
    static constexpr int kWidth = 128;

    MppLevel();
    virtual void Generate(MppWorld& world, int level);
    void Load(MppWorld& world, Savepoint& savepoint, int level);
    void Save(Savepoint& savepoint, int level, bool all);
    void Update(MppWorld& world, MppRenderer& renderer, int ticks);
    void AddEntity(const std::shared_ptr<MppEntity>& entity);
    const MppTile& GetTile(int x, int y) const;
    bool IsValid(int x, int y) const;

protected:
    virtual MppTileID Generate(float x, float y) const = 0;

private:
    std::array<std::array<MppTile, kWidth>, kWidth> Tiles;
    std::vector<std::shared_ptr<MppEntity>> Entities;
    int MinDirtyX;
    int MinDirtyY;
    int MaxDirtyX;
    int MaxDirtyY;
};