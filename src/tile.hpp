#pragma once

#include <savepoint_fwd.hpp>

#include "physics.hpp"

enum MppTileID
{
    MppTileIDGrass,
    MppTileIDDirt,
    MppTileIDStone,
    MppTileIDTree,
    MppTileIDSand,
    MppTileIDCount,
    MppTileIDInvalid = MppTileIDCount,
};

class MppLevel;
class MppRenderer;

class MppTile
{
public:
    static constexpr int kWidth = 16;

    constexpr MppTile() : ID{MppTileIDInvalid} {}
    MppTile(MppTileID id);
    void Update(MppLevel& level, MppRenderer& renderer, int x, int y, float ticks);
    void Visit(SavepointVisitor& visitor);
    MppTileID GetID() const;
    MppPhysicsType GetPhysicsType() const;
    int GetPhysicsX(int x) const;
    int GetPhysicsY(int y) const;
    int GetPhysicsWidth() const;
    int GetPhysicsHeight() const;
    bool operator==(const MppTile& other) const;
    bool IsValid() const;

private:
    void Draw(const MppLevel& level, MppRenderer& renderer, int x, int y);
    void DrawPhysics(MppRenderer& renderer, int x, int y);

    MppTileID ID;
};

static constexpr MppTile kMppTileInvalid{};