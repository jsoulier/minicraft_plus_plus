#pragma once

#include <savepoint/savepoint.hpp>

#include <array>
#include <cstdint>
#include <memory>
#include <string_view>

class MppEntity;
enum MppEntityCollision : uint8_t;

enum MppTileID : uint16_t
{
    MppTileIDInvalid,
    MppTileIDGrass,
    MppTileIDDirt,
    MppTileIDStoneWall,
    MppTileIDTree,
    MppTileIDSand,
    MppTileIDStairsDown,
    MppTileIDStairsUp,
    MppTileIDStone,
    MppTileIDIronRails,
    MppTileIDCount,
};

std::string_view MppTileIDGetName(MppTileID id);

enum MppTilePhysicsType
{
    // Physics MUST be disjoint. e.g. Cannot have Ground and Wall at the same time
    MppTilePhysicsTypeGround = 1,
    MppTilePhysicsTypeWall   = 2,
    MppTilePhysicsTypeLiquid = 4,
    MppTilePhysicsTypeStairs = 8,
    MppTilePhysicsTypeRails  = 16,
};

enum MppTileLayer
{
    MppTileLayerBottom,
    MppTileLayerTop,
    MppTileLayerCount,
};

class MppTile
{
public:
    static constexpr int kSize = 16;

    MppTile();
    MppTile(MppTileID bottomID);
    MppTile(MppTileID bottomID, MppTileID topID);
    void Visit(SavepointVisitor& visitor);
    void Update(int x, int y, uint64_t ticks);
    void Render(int x, int y) const;
    bool OnAction(std::shared_ptr<MppEntity>& instigator, int x, int y);
    bool OnInteraction(std::shared_ptr<MppEntity>& instigator, int x, int y);
    MppEntityCollision OnCollision(const std::shared_ptr<MppEntity>& instigator, int x, int y);
    MppTileID GetID(MppTileLayer layer) const;
    MppTileID GetID() const;
    MppTilePhysicsType GetPhysicsType() const;
    int GetPhysicsX(int x) const;
    int GetPhysicsY(int y) const;
    int GetPhysicsWidth() const;
    int GetPhysicsHeight() const;
    bool operator==(const MppTile& other) const;
    bool IsValid() const;

private:
    MppTileID& GetMutableID();

    std::array<MppTileID, MppTileLayerCount> Layers;
    uint64_t Ticks;
};

static MppTile kMppTileInvalid{};
static MppTileID kMppTileIDInvalid{kMppTileIDInvalid};
