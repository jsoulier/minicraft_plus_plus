#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <limits>
#include <memory>
#include <string_view>

#include <minicraft++/assert.hpp>
#include <minicraft++/audio.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/console.hpp>
#include <minicraft++/entity/entity.hpp>
#include <minicraft++/entity/mob/vehicle.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/entity/particle/hit.hpp>
#include <minicraft++/entity/particle/particle.hpp>
#include <minicraft++/inventory.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/sprite.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/world.hpp>

static constexpr uint64_t kInvalidTicks = std::numeric_limits<uint64_t>::max();
static constexpr int kDirtColor = 420;

static const MppAudioHandle kDestroyedAudio{{"destroyed_tile_1", "destroyed_tile_2"}};
static const MppAudioHandle kPlacedAudio{{"placed_tile_1"}};

enum TileSpriteType
{
    TileSpriteType1x1,
    TileSpriteType2x2,
    TileSpriteType1x2_2x1,
    TileSpriteTypeNoLinks,
};

struct
{
    std::string_view Name;
    TileSpriteType SpriteType;
    int SpriteX;
    int SpriteY;
    int TopSpriteY;
    int Color1;
    int Color2;
    int Color3;
    int Color4;
    int Color5;
    MppTilePhysicsType PhysicsType;
    int PhysicsOffsetX;
    int PhysicsOffsetY;
    int PhysicsWidth;
    int PhysicsHeight;
    MppTileID ChildTile;
    MppItemType ItemType;
}
static constexpr kTiles[MppTileIDCount] =
{
    {
        .Name = "invalid",
        .SpriteType = TileSpriteType1x1,
        .SpriteX = 0,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .Color1 = 0,
        .Color2 = 0,
        .Color3 = 0,
        .Color4 = 0,
        .Color5 = 0,
        .PhysicsType = MppTilePhysicsTypeWall,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
        .ChildTile = MppTileIDInvalid,
        .ItemType = MppItemTypeNone,
    },
    {
        .Name = "grass",
        .SpriteType = TileSpriteType1x1,
        .SpriteX = 0,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .Color1 = 32,
        .Color2 = 52,
        .Color3 = 20,
        .Color4 = kDirtColor,
        .Color5 = 0,
        .PhysicsType = MppTilePhysicsTypeGround,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
        .ChildTile = MppTileIDDirt,
        .ItemType = MppItemTypeShovel | MppItemTypeHoe,
    },
    {
        .Name = "dirt",
        .SpriteType = TileSpriteType1x1,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .Color1 = kDirtColor,
        .Color2 = kDirtColor,
        .Color3 = kDirtColor,
        .Color4 = kDirtColor,
        .Color5 = 0,
        .PhysicsType = MppTilePhysicsTypeGround,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
        .ChildTile = MppTileIDInvalid,
        .ItemType = MppItemTypeNone,
    },
    {
        .Name = "stone_wall",
        .SpriteType = TileSpriteType1x2_2x1,
        .SpriteX = 0,
        .SpriteY = 3,
        .TopSpriteY = 4,
        .Color1 = 333,
        .Color2 = 111,
        .Color3 = 555,
        .Color4 = 444,
        .Color5 = 0,
        .PhysicsType = MppTilePhysicsTypeWall,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
        .ChildTile = MppTileIDInvalid,
        .ItemType = MppItemTypePickaxe | MppItemTypeHands,
    },
    {
        .Name = "tree",
        .SpriteType = TileSpriteType2x2,
        .SpriteX = 0,
        .SpriteY = 5,
        .TopSpriteY = 0,
        .Color1 = 20,
        .Color2 = 52,
        .Color3 = 430,
        .Color4 = 320,
        .Color5 = 252,
        .PhysicsType = MppTilePhysicsTypeWall,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
        .ChildTile = MppTileIDInvalid,
        .ItemType = MppItemTypeAxe | MppItemTypeHands,
    },
    {
        .Name = "sand",
        .SpriteType = TileSpriteType1x1,
        .SpriteX = 0,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .Color1 = 550,
        .Color2 = 550,
        .Color3 = 550,
        .Color4 = kDirtColor,
        .Color5 = 0,
        .PhysicsType = MppTilePhysicsTypeGround,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
        .ChildTile = MppTileIDDirt,
        .ItemType = MppItemTypeShovel,
    },
    {
        .Name = "stairs_down",
        .SpriteType = TileSpriteTypeNoLinks,
        .SpriteX = 0,
        .SpriteY = 19,
        .TopSpriteY = 0,
        .Color1 = 0,
        .Color2 = 222,
        .Color3 = 333,
        .Color4 = 0,
        .Color5 = kDirtColor,
        .PhysicsType = MppTilePhysicsTypeStairs,
        .PhysicsOffsetX = 7,
        .PhysicsOffsetY = 7,
        .PhysicsWidth = 2,
        .PhysicsHeight = 2,
        .ChildTile = MppTileIDInvalid,
        .ItemType = MppItemTypeNone,
    },
    {
        .Name = "stairs_up",
        .SpriteType = TileSpriteTypeNoLinks,
        .SpriteX = 1,
        .SpriteY = 19,
        .TopSpriteY = 0,
        .Color1 = 0,
        .Color2 = 222,
        .Color3 = 333,
        .Color4 = 0,
        .Color5 = kDirtColor,
        .PhysicsType = MppTilePhysicsTypeStairs,
        .PhysicsOffsetX = 7,
        .PhysicsOffsetY = 7,
        .PhysicsWidth = 2,
        .PhysicsHeight = 2,
        .ChildTile = MppTileIDInvalid,
        .ItemType = MppItemTypeNone,
    },
    {
        .Name = "stone",
        .SpriteType = TileSpriteType1x1,
        .SpriteX = 0,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .Color1 = 333,
        .Color2 = 333,
        .Color3 = 333,
        .Color4 = kDirtColor,
        .Color5 = 0,
        .PhysicsType = MppTilePhysicsTypeGround,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
        .ChildTile = MppTileIDInvalid,
        .ItemType = MppItemTypeNone,
    },
    {
        .Name = "iron_rails",
        .SpriteType = TileSpriteType1x1,
        .SpriteX = 0,
        .SpriteY = 20,
        .TopSpriteY = 0,
        .Color1 = 111,
        .Color2 = 222,
        .Color3 = 210,
        .Color4 = 0,
        .Color5 = 0,
        .PhysicsType = MppTilePhysicsTypeRails,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
        .ChildTile = MppTileIDInvalid,
        .ItemType = MppItemTypeEquipment,
    },
    {
        .Name = "water",
        .SpriteType = TileSpriteType1x1,
        .SpriteX = 0,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .Color1 = 5,
        .Color2 = 3,
        .Color3 = 4,
        .Color4 = kDirtColor,
        .Color5 = 0,
        .PhysicsType = MppTilePhysicsTypeLiquid,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
        .ChildTile = MppTileIDInvalid,
        .ItemType = MppItemTypeNone,
    },
};

std::string_view MppTileIDGetName(MppTileID id)
{
    return kTiles[id].Name;
}

/**
 * 0: has north neighbor
 * 1: has south neighbor
 * 2: has east neighbor
 * 3: has west neighbor
 */
static constexpr int kSprites[] =
{
    /* 0b0000 */ 15,
    /* 0b0001 */ 14,
    /* 0b0010 */ 13,
    /* 0b0011 */ 9,
    /* 0b0100 */ 11,
    /* 0b0101 */ 2,
    /* 0b0110 */ 1,
    /* 0b0111 */ 5,
    /* 0b1000 */ 12,
    /* 0b1001 */ 3,
    /* 0b1010 */ 4,
    /* 0b1011 */ 6,
    /* 0b1100 */ 10,
    /* 0b1101 */ 8,
    /* 0b1110 */ 7,
    /* 0b1111 */ 0,
};

static int GetSprite1x1(MppTileID lhs, int x, int y, MppTileLayer layer)
{
    int hash = 0;
    hash |= (lhs == MppWorldGetTile(x + 0, y - 1).GetID(layer)) << 0;
    hash |= (lhs == MppWorldGetTile(x + 0, y + 1).GetID(layer)) << 1;
    hash |= (lhs == MppWorldGetTile(x + 1, y + 0).GetID(layer)) << 2;
    hash |= (lhs == MppWorldGetTile(x - 1, y + 0).GetID(layer)) << 3;
    return kSprites[hash];
}

/**
 * 0: has north-west neighbor
 * 1: has south-west neighbor
 * 2: has south-east neighbor
 * 3: has north-east neighbor
 */
static constexpr int kSprites2x2[] =
{
    /* 0b0000 */ 0,
    /* 0b0001 */ 1,
    /* 0b0010 */ 2,
    /* 0b0011 */ 5,
    /* 0b0100 */ 3,
    /* 0b0101 */ 10,
    /* 0b0110 */ 8,
    /* 0b0111 */ 13,
    /* 0b1000 */ 4,
    /* 0b1001 */ 7,
    /* 0b1010 */ 9,
    /* 0b1011 */ 11,
    /* 0b1100 */ 6,
    /* 0b1101 */ 12,
    /* 0b1110 */ 14,
    /* 0b1111 */ 15,
};

static int GetSprite2x2(MppTileID lhs, int x, int y, MppTileLayer layer)
{
    int hash = 0;
    hash |=
        ((lhs == MppWorldGetTile(x - 1, y + 0).GetID(layer)) &&
        (lhs == MppWorldGetTile(x + 0, y - 1).GetID(layer)) &&
        (lhs == MppWorldGetTile(x - 1, y - 1).GetID(layer))) << 0;
    hash |=
        ((lhs == MppWorldGetTile(x - 1, y + 0).GetID(layer)) &&
        (lhs == MppWorldGetTile(x + 0, y + 1).GetID(layer)) &&
        (lhs == MppWorldGetTile(x - 1, y + 1).GetID(layer))) << 1;
    hash |=
        ((lhs == MppWorldGetTile(x + 1, y + 0).GetID(layer)) &&
        (lhs == MppWorldGetTile(x + 0, y + 1).GetID(layer)) &&
        (lhs == MppWorldGetTile(x + 1, y + 1).GetID(layer))) << 2;
    hash |=
        ((lhs == MppWorldGetTile(x + 1, y + 0).GetID(layer)) &&
        (lhs == MppWorldGetTile(x + 0, y - 1).GetID(layer)) &&
        (lhs == MppWorldGetTile(x + 1, y - 1).GetID(layer))) << 3;
    return kSprites2x2[hash];
}

static int GetSprite1x2_2x1(MppTileID lhs, int x, int y, MppTileLayer layer)
{
    int hash = 0;
    hash |=
        ((lhs == MppWorldGetTile(x - 1, y + 0).GetID(layer)) &&
        (lhs == MppWorldGetTile(x + 0, y - 1).GetID(layer)) &&
        (lhs != MppWorldGetTile(x - 1, y - 1).GetID(layer))) << 0;
    hash |=
        ((lhs == MppWorldGetTile(x - 1, y + 0).GetID(layer)) &&
         (lhs == MppWorldGetTile(x + 0, y + 1).GetID(layer)) &&
         (lhs != MppWorldGetTile(x - 1, y + 1).GetID(layer))) << 1;
    hash |=
        ((lhs == MppWorldGetTile(x + 1, y + 0).GetID(layer)) &&
        (lhs == MppWorldGetTile(x + 0, y + 1).GetID(layer)) &&
        (lhs != MppWorldGetTile(x + 1, y + 1).GetID(layer))) << 2;
    hash |=
        ((lhs == MppWorldGetTile(x + 1, y + 0).GetID(layer)) &&
        (lhs == MppWorldGetTile(x + 0, y - 1).GetID(layer)) &&
        (lhs != MppWorldGetTile(x + 1, y - 1).GetID(layer))) << 3;
    return kSprites2x2[hash];
}

MppTile::MppTile()
    : Layers{MppTileIDInvalid, MppTileIDInvalid}
    , Ticks{kInvalidTicks}
{
}

MppTile::MppTile(MppTileID bottomID)
    : Layers{bottomID, MppTileIDInvalid}
    , Ticks{kInvalidTicks}
{
}

MppTile::MppTile(MppTileID bottomID, MppTileID topID)
    : Layers{bottomID, topID}
    , Ticks{kInvalidTicks}
{
}

void MppTile::Visit(SavepointVisitor& visitor)
{
    visitor(Layers);
    visitor(Ticks);
}

void MppTile::Update(int x, int y, uint64_t ticks)
{
}

void MppTile::Render(int x, int y) const
{
    if (MppConsole::CVarPhysics.GetBool())
    {
        int color = 0;
        x = GetPhysicsX(x);
        y = GetPhysicsY(y);
        int w = GetPhysicsWidth();
        int h = GetPhysicsHeight();
        if (GetPhysicsType() == MppTilePhysicsTypeWall)
        {
            color = kMppColorDebugWallTilePhysics;
        }
        else if (GetPhysicsType() == MppTilePhysicsTypeLiquid)
        {
            color = kMppColorDebugLiquidTilePhysics;
        }
        else if (GetPhysicsType() == MppTilePhysicsTypeStairs)
        {
            color = kMppColorDebugStairsTilePhysics;
        }
        if (color)
        {
            MppRendererDrawRect(color, x, y, w, h, MppRendererLayerDebugTilePhysics);
        }
    }
    MppTileID bottomID = GetID(MppTileLayerBottom);
    MppTileID topID = GetID(MppTileLayerTop);
    if (kTiles[bottomID].SpriteType == TileSpriteType1x1)
    {
        MppRendererDraw(
            MppSprite{
                kTiles[bottomID].Color1,
                kTiles[bottomID].Color2,
                kTiles[bottomID].Color3,
                kTiles[bottomID].Color4,
                kTiles[bottomID].Color5,
                kTiles[bottomID].SpriteX + GetSprite1x1(bottomID, x, y, MppTileLayerBottom),
                kTiles[bottomID].SpriteY,
                MppTile::kSize,
            },
            x * MppTile::kSize,
            y * MppTile::kSize,
            MppRendererModNone,
            MppRendererLayerBottomTile);
    }
    else if (kTiles[bottomID].SpriteType == TileSpriteTypeNoLinks)
    {
        MppRendererDraw(
            MppSprite{
                kTiles[bottomID].Color1,
                kTiles[bottomID].Color2,
                kTiles[bottomID].Color3,
                kTiles[bottomID].Color4,
                kTiles[bottomID].Color5,
                kTiles[bottomID].SpriteX,
                kTiles[bottomID].SpriteY,
                MppTile::kSize,
            },
            x * MppTile::kSize,
            y * MppTile::kSize,
            MppRendererModNone,
            MppRendererLayerBottomTile);
    }
    else
    {
        MppAssert(false);
    }
    if (topID == MppTileIDInvalid)
    {
        return;
    }
    else if (kTiles[topID].SpriteType == TileSpriteType1x1)
    {
        MppRendererDraw(
            MppSprite{
                kTiles[topID].Color1,
                kTiles[topID].Color2,
                kTiles[topID].Color3,
                kTiles[topID].Color4,
                kTiles[topID].Color5,
                kTiles[topID].SpriteX + GetSprite1x1(topID, x, y, MppTileLayerTop),
                kTiles[topID].SpriteY,
                MppTile::kSize,
            },
            x * MppTile::kSize,
            y * MppTile::kSize,
            MppRendererModNone,
            MppRendererLayerBottomTile);
    }
    else if (kTiles[topID].SpriteType == TileSpriteType2x2)
    {
        SDL_assert(kTiles[topID].TopSpriteY == 0);
        MppRendererDraw(
            MppSprite{
                kTiles[topID].Color1,
                kTiles[topID].Color2,
                kTiles[topID].Color3,
                kTiles[topID].Color4,
                kTiles[topID].Color5,
                kTiles[topID].SpriteX + GetSprite2x2(topID, x, y, MppTileLayerTop),
                kTiles[topID].SpriteY,
                MppTile::kSize,
            },
            x * MppTile::kSize,
            y * MppTile::kSize,
            MppRendererModNone,
            MppRendererLayerTile);
    }
    else if (kTiles[topID].SpriteType == TileSpriteType1x2_2x1)
    {
        MppRendererDraw(
            MppSprite{
                kTiles[topID].Color1,
                kTiles[topID].Color2,
                kTiles[topID].Color3,
                kTiles[topID].Color4,
                kTiles[topID].Color5,
                kTiles[topID].SpriteX + GetSprite1x1(topID, x, y, MppTileLayerTop),
                kTiles[topID].SpriteY,
                MppTile::kSize,
            },
            x * MppTile::kSize,
            y * MppTile::kSize,
            MppRendererModNone,
            MppRendererLayerTile);
        MppRendererDraw(
            MppSprite{
                kTiles[topID].Color1,
                kTiles[topID].Color2,
                kTiles[topID].Color3,
                kTiles[topID].Color4,
                kTiles[topID].Color5,
                kTiles[topID].SpriteX + GetSprite1x2_2x1(topID, x, y, MppTileLayerTop),
                kTiles[topID].TopSpriteY,
                MppTile::kSize,
            },
            x * MppTile::kSize,
            y * MppTile::kSize,
            MppRendererModNone,
            MppRendererLayerTopTile);
    }
    else
    {
        MppAssert(false);
    }
}

bool MppTile::OnAction(std::shared_ptr<MppEntity>& instigator, int x, int y)
{
    if (!IsValid())
    {
        return false;
    }
    std::shared_ptr<MppMobEntity> mob = instigator->Cast<MppMobEntity>();
    if (!mob)
    {
        MppLog("A non-mob entity tried to perform an action on a tile");
        return false;
    }
    MppItem item = mob->GetInventory()->GetBySlot(MppInventorySlotHeld);
    MppAssert(item.IsValid());
    if (item.GetType() & MppItemTypeEquipment)
    {
        MppTileID& id = GetMutableID();
        if ((item.GetType() & kTiles[id].ItemType) == MppItemTypeNone)
        {
            return false;
        }
        id = kTiles[id].ChildTile;
        kDestroyedAudio.Play();
        std::shared_ptr<MppEntity> entity = MppEntity::Create<MppHitEntity>();
        entity->SetX(x * kSize);
        entity->SetY(y * kSize);
        MppWorldAddEntity(entity);
    }
    else if (item.GetType() & MppItemTypeConsumable)
    {
        if (Layers[MppTileLayerTop] != MppTileIDInvalid)
        {
            return false;
        }
        // TODO: not everything supports
        MppTileID id = item.GetTileID();
        if (id == MppTileIDInvalid)
        {
            return false;
        }
        Layers[MppTileLayerTop] = id;
        kPlacedAudio.Play();
    }
    return true;
}

bool MppTile::OnInteraction(std::shared_ptr<MppEntity>& instigator, int x, int y)
{
    return false;
}

MppEntityCollision MppTile::OnCollision(const std::shared_ptr<MppEntity>& instigator, int x, int y)
{
    MppTileID id = GetID();
    std::shared_ptr<MppVehicleEntity> vehicle = instigator->Cast<MppVehicleEntity>();
    if (vehicle && vehicle->GetPhysicsType() != GetPhysicsType())
    {
        return MppEntityCollisionReject;
    }
    if (GetPhysicsType() == MppTilePhysicsTypeStairs)
    {
        // TODO: If a held entity is placed. Pretty dangerous edge case, should fix properly
        if (!instigator->IsSpawned())
        {
            return MppEntityCollisionReject;
        }
        int level = MppWorldGetLevel();
        const int (*positions)[4][2] = nullptr;
        if (id == MppTileIDStairsDown)
        {
            static constexpr int kPositions[4][2] = {{-1, 0}, {0, 1}, {0, -1}, {1, 0}};
            positions = &kPositions;
            level++;
        }
        else if (id == MppTileIDStairsUp)
        {
            static constexpr int kPositions[4][2] = {{1, 0}, {0, 1}, {0, -1}, {-1, 0}};
            positions = &kPositions;
            level--;
        }
        else
        {
            MppAssert(false);
        }
        int i = 0;
        for (; i < 4; i++)
        {
            int tx = x + (*positions)[i][0];
            int ty = y + (*positions)[i][1];
            const MppTile& tile = MppWorldGetTile(tx, ty, level);
            if (tile.GetPhysicsType() == MppTilePhysicsTypeGround)
            {
                instigator->SetX(tx * kSize);
                instigator->SetY(ty * kSize);
                break;
            }
        }
        if (i >= 4)
        {
            MppLog("No valid positions found for entity using stairs");
            return MppEntityCollisionReject;
        }
        MppWorldSetEntityLevel(instigator, level);
        return MppEntityCollisionTeleport;
    }
    else if (GetPhysicsType() == MppTilePhysicsTypeWall)
    {
        return MppEntityCollisionReject;
    }
    else
    {
        return MppEntityCollisionAccept;
    }
}

MppTileID MppTile::GetID(MppTileLayer layer) const
{
    return Layers[layer];
}

MppTilePhysicsType MppTile::GetPhysicsType() const
{
    return kTiles[GetID()].PhysicsType;
}

int MppTile::GetPhysicsX(int x) const
{
    return x * kSize + kTiles[GetID()].PhysicsOffsetX;
}

int MppTile::GetPhysicsY(int y) const
{
    return y * kSize + kTiles[GetID()].PhysicsOffsetY;
}

int MppTile::GetPhysicsWidth() const
{
    return kTiles[GetID()].PhysicsWidth;
}

int MppTile::GetPhysicsHeight() const
{
    return kTiles[GetID()].PhysicsHeight;
}

bool MppTile::operator==(const MppTile& other) const
{
    return GetID() == other.GetID();
}

bool MppTile::IsValid() const
{
    return GetID() != MppTileIDInvalid;
}

MppTileID MppTile::GetID() const
{
    for (auto it = Layers.rbegin(); it != Layers.rend(); it++)
    {
        if (*it != MppTileIDInvalid)
        {
            return *it;
        }
    }
    return MppTileIDInvalid;
}

MppTileID& MppTile::GetMutableID()
{
    for (auto it = Layers.rbegin(); it != Layers.rend(); it++)
    {
        if (*it != MppTileIDInvalid)
        {
            return *it;
        }
    }
    return kMppTileIDInvalid;
}
