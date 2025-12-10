#pragma once

#include <savepoint.hpp>

enum MppTileType
{
    MppTileTypeInvalid,
    MppTileTypeGrass,
    MppTileTypeCount,
};

class MppTile
{
public:
    constexpr MppTile() : Type{MppTileTypeInvalid} {}
    void Visit(SavepointVisitor& visitor);
    MppTileType GetType() const;
    bool operator==(const MppTile& other) const;
    bool IsValid() const;

private:
    MppTileType Type;
};

static constexpr MppTile kMppTileInvalid{};