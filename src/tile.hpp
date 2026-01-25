#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>

class MppEntity;

enum MppTileID : uint16_t
{
    MppTileIDInvalid,
    MppTileIDGrass,
    MppTileIDDirt,
    MppTileIDStoneWall,
    MppTileIDTree,
    MppTileIDSand,
    MppTileIDCount,
};

enum MppTilePhysicsType
{
    MppTilePhysicsTypeGround,
    MppTilePhysicsTypeWall,
    MppTilePhysicsTypeLiquid,
};

class MppTile
{
public:
    static constexpr int kSize = 16;

    MppTile();
    MppTile(MppTileID id);
    void Visit(SavepointVisitor& visitor);
    void Update(int x, int y, uint64_t ticks);
    void Render(int x, int y);
    void OnAction(MppEntity& instigator);
    void OnCollision(MppEntity& instigator);
    MppTileID GetID() const;
    MppTilePhysicsType GetPhysicsType() const;
    int GetPhysicsX(int x) const;
    int GetPhysicsY(int y) const;
    int GetPhysicsWidth() const;
    int GetPhysicsHeight() const;
    bool operator==(const MppTile& other) const;
    bool IsValid() const;

private:
    MppTileID ID;
    uint64_t Ticks;
};

static MppTile kMppTileInvalid{};