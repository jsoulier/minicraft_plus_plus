#pragma once

#include <savepoint.hpp>

#include <array>
#include <memory>
#include <vector>

#include "entity.hpp"
#include "tile.hpp"

class MppLevel
{
public:
    static constexpr int kWidth = 256;

    MppLevel();
    void Generate();
    void Load(Savepoint& savepoint, int level);
    void AddEntity(const std::shared_ptr<MppEntity>& entity);
    void SetTile(MppTile tile, int x, int y);
    const MppTile& GetTile(int x, int y) const;
    bool IsValid(int x, int y) const;

protected:
    virtual MppTileType Generate(int x, int y) const = 0;

private:
    std::array<std::array<MppTile, kWidth>, kWidth> Tiles;
    std::vector<std::shared_ptr<MppEntity>> Entities;
};