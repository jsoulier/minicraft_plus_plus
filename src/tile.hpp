#pragma once

#include <savepoint.hpp>

#include "physics.hpp"

enum MppTileType
{
    MppTileTypeGrass,
    MppTileTypeDirt,
    MppTileTypeStone,
    MppTileTypeTree,
    MppTileTypeSand,
    MppTileTypeCount,
    MppTileTypeInvalid = MppTileTypeCount,
};

class MppLevel;
class MppRenderer;

class MppTile
{
public:
    static constexpr int kWidth = 16;

    constexpr MppTile() : Type{MppTileTypeInvalid} {}
    MppTile(MppTileType type);
    void Update(MppLevel& level, MppRenderer& renderer, int x, int y, float ticks);
    void Visit(SavepointVisitor& visitor);
    MppTileType GetType() const;
    MppPhysicsType GetPhysicsType() const;
    int GetPhysicsX(int x) const;
    int GetPhysicsY(int y) const;
    int GetPhysicsWidth() const;
    int GetPhysicsHeight() const;
    bool operator==(const MppTile& other) const;
    bool IsValid() const;

private:
    void Render(const MppLevel& level, MppRenderer& renderer, int x, int y);
    void RenderPhysics(MppRenderer& renderer, int x, int y);

    MppTileType Type;
};

static constexpr MppTile kMppTileInvalid{};