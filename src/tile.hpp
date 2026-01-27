#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <string_view>

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
    // Physics MUST be disjoint. e.g. Cannot have Ground and Wall at the same time
    MppTilePhysicsTypeGround = 0x01,
    MppTilePhysicsTypeWall   = 0x02,
    MppTilePhysicsTypeLiquid = 0x04,
};

class MppTile
{
public:
    static constexpr int kSize = 16;

    MppTile();
    MppTile(MppTileID id);
    void Visit(SavepointVisitor& visitor);
    void Update(int x, int y, uint64_t ticks);
    void Render(int x, int y) const;
    void OnAction(MppEntity& instigator);
    void OnCollision(MppEntity& instigator);
    const std::string_view& GetName() const;
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