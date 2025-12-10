#pragma once

#include <savepoint.hpp>

enum MppTileType
{
    MppTileTypeInvalid,
    MppTileTypeDirt,
    MppTileTypeStone,
    MppTileTypeTree,
    MppTileTypeSand,
    MppTileTypeGrass,
    MppTileTypeCount,
};

class MppLevel;
class MppRenderer;

class MppTile
{
public:
    static constexpr int kWidth = 16;

    constexpr MppTile() : Type{MppTileTypeInvalid} {}
    MppTile(MppTileType type);
    void Update(MppLevel& level, MppRenderer& renderer, int x, int y, float dt, float ticks);
    void Visit(SavepointVisitor& visitor);
    MppTileType GetType() const;
    bool operator==(const MppTile& other) const;
    bool IsValid() const;

private:
    MppTileType Type;
};

static constexpr MppTile kMppTileInvalid{};