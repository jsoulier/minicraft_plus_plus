#include <cstdint>
#include <limits>

#include "assert.hpp"
#include "color.hpp"
#include "entity.hpp"
#include "item.hpp"
#include "renderer.hpp"
#include "sprite.hpp"
#include "tile.hpp"
#include "world.hpp"

static constexpr uint64_t kInvalidTicks = std::numeric_limits<uint64_t>::max();

enum TileSpriteType
{
    TileSpriteType1x1,
    TileSpriteType2x2,
    TileSpriteType1x2_2x1,
};

struct
{
    TileSpriteType SpriteType;
    int SpriteY;
    int TopSpriteY;
    MppTileID BottomTile;
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
}
static constexpr kTiles[MppTileIDCount] =
{
    /* invalid */
    {
        .SpriteType = TileSpriteType1x1,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .BottomTile = MppTileIDCount,
        .Color1 = 0,
        .Color2 = 0,
        .Color3 = 0,
        .Color4 = 0,
        .Color5 = 0,
        .PhysicsType = MppTilePhysicsTypeGround,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
    },
    /* grass */
    {
        .SpriteType = TileSpriteType1x1,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .BottomTile = MppTileIDCount,
        .Color1 = 62,
        .Color2 = 82,
        .Color3 = 40,
        .Color4 = kMppColorDirt,
        .Color5 = 0,
        .PhysicsType = MppTilePhysicsTypeGround,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
    },
    /* dirt */
    {
        .SpriteType = TileSpriteType1x1,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .BottomTile = MppTileIDCount,
        .Color1 = kMppColorDirt,
        .Color2 = kMppColorDirt,
        .Color3 = kMppColorDirt,
        .Color4 = kMppColorDirt,
        .Color5 = 0,
        .PhysicsType = MppTilePhysicsTypeGround,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
    },
    /* stone wall */
    {
        .SpriteType = TileSpriteType1x2_2x1,
        .SpriteY = 3,
        .TopSpriteY = 4,
        .BottomTile = MppTileIDCount,
        .Color1 = kMppColorStone1,
        .Color2 = kMppColorStone2,
        .Color3 = kMppColorStone3,
        .Color4 = kMppColorStone4,
        .Color5 = kMppColorDirt,
        .PhysicsType = MppTilePhysicsTypeWall,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
    },
    /* tree */
    {
        .SpriteType = TileSpriteType2x2,
        .SpriteY = 5,
        .TopSpriteY = 0,
        .BottomTile = MppTileIDGrass,
        .Color1 = 40,
        .Color2 = 82,
        .Color3 = kMppColorWood2,
        .Color4 = kMppColorWood1,
        .Color5 = 494,
        .PhysicsType = MppTilePhysicsTypeWall,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
    },
    /* sand */
    {
        .SpriteType = TileSpriteType1x1,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .BottomTile = MppTileIDCount,
        .Color1 = 990,
        .Color2 = 990,
        .Color3 = 990,
        .Color4 = kMppColorDirt,
        .Color5 = 0,
        .PhysicsType = MppTilePhysicsTypeGround,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kSize,
        .PhysicsHeight = MppTile::kSize,
    },
};

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

static bool Equals1x1(MppTileID lhs, MppTileID rhs)
{
    return lhs == rhs || kTiles[lhs].BottomTile == rhs || lhs == kTiles[rhs].BottomTile;
}

static int GetSprite1x1(MppTileID lhs, int x, int y)
{
    int hash = 0;
    hash |= Equals1x1(lhs, MppWorldGetTile(x + 0, y - 1).GetID()) << 0;
    hash |= Equals1x1(lhs, MppWorldGetTile(x + 0, y + 1).GetID()) << 1;
    hash |= Equals1x1(lhs, MppWorldGetTile(x + 1, y + 0).GetID()) << 2;
    hash |= Equals1x1(lhs, MppWorldGetTile(x - 1, y + 0).GetID()) << 3;
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

static bool Equals2x2(MppTileID lhs, MppTileID rhs)
{
    return lhs == rhs;
}

static int GetSprite2x2(MppTileID lhs, int x, int y)
{
    int hash = 0;
    hash |=
        (Equals2x2(lhs, MppWorldGetTile(x - 1, y + 0).GetID()) &&
         Equals2x2(lhs, MppWorldGetTile(x + 0, y - 1).GetID()) &&
         Equals2x2(lhs, MppWorldGetTile(x - 1, y - 1).GetID())) << 0;
    hash |=
        (Equals2x2(lhs, MppWorldGetTile(x - 1, y + 0).GetID()) &&
         Equals2x2(lhs, MppWorldGetTile(x + 0, y + 1).GetID()) &&
         Equals2x2(lhs, MppWorldGetTile(x - 1, y + 1).GetID())) << 1;
    hash |=
        (Equals2x2(lhs, MppWorldGetTile(x + 1, y + 0).GetID()) &&
         Equals2x2(lhs, MppWorldGetTile(x + 0, y + 1).GetID()) &&
         Equals2x2(lhs, MppWorldGetTile(x + 1, y + 1).GetID())) << 2;
    hash |=
        (Equals2x2(lhs, MppWorldGetTile(x + 1, y + 0).GetID()) &&
         Equals2x2(lhs, MppWorldGetTile(x + 0, y - 1).GetID()) &&
         Equals2x2(lhs, MppWorldGetTile(x + 1, y - 1).GetID())) << 3;
    return kSprites2x2[hash];
}

static int GetSprite1x2_2x1(MppTileID lhs, int x, int y)
{
    int hash = 0;
    hash |=
        (Equals2x2(lhs, MppWorldGetTile(x - 1, y + 0).GetID()) &&
         Equals2x2(lhs, MppWorldGetTile(x + 0, y - 1).GetID()) &&
        !Equals2x2(lhs, MppWorldGetTile(x - 1, y - 1).GetID())) << 0;
    hash |=
        (Equals2x2(lhs, MppWorldGetTile(x - 1, y + 0).GetID()) &&
         Equals2x2(lhs, MppWorldGetTile(x + 0, y + 1).GetID()) &&
        !Equals2x2(lhs, MppWorldGetTile(x - 1, y + 1).GetID())) << 1;
    hash |=
        (Equals2x2(lhs, MppWorldGetTile(x + 1, y + 0).GetID()) &&
         Equals2x2(lhs, MppWorldGetTile(x + 0, y + 1).GetID()) &&
        !Equals2x2(lhs, MppWorldGetTile(x + 1, y + 1).GetID())) << 2;
    hash |=
        (Equals2x2(lhs, MppWorldGetTile(x + 1, y + 0).GetID()) &&
         Equals2x2(lhs, MppWorldGetTile(x + 0, y - 1).GetID()) &&
        !Equals2x2(lhs, MppWorldGetTile(x + 1, y - 1).GetID())) << 3;
    return kSprites2x2[hash];
}

MppTile::MppTile()
    : ID{MppTileIDInvalid}
    , Ticks{kInvalidTicks}
{
}

MppTile::MppTile(MppTileID id)
    : ID{id}
    , Ticks{kInvalidTicks}
{
}

void MppTile::Visit(SavepointVisitor& visitor)
{
    visitor(ID);
    visitor(Ticks);
}

void MppTile::Update(int x, int y, uint64_t ticks)
{
    if (Ticks == kInvalidTicks)
    {
        Ticks = ticks;
    }
    while (Ticks < ticks)
    {
        // TODO:
        // Ticks += time until next tick
        // TODO: remove
        Ticks = ticks;
    }
}

void MppTile::Render(int x, int y)
{
    if (kTiles[ID].SpriteType == TileSpriteType1x1)
    {
        MppRendererDraw(
            MppSprite{
                kTiles[ID].Color1,
                kTiles[ID].Color2,
                kTiles[ID].Color3,
                kTiles[ID].Color4,
                kTiles[ID].Color5,
                GetSprite1x1(ID, x, y),
                kTiles[ID].SpriteY,
                MppTile::kSize,
            },
            x * MppTile::kSize,
            y * MppTile::kSize,
            false,
            MppRendererLayerTile);
    }
    else if (kTiles[ID].SpriteType == TileSpriteType2x2)
    {
        SDL_assert(kTiles[ID].TopSpriteY == 0);
        if (kTiles[ID].BottomTile != MppTileIDCount)
        {
            MppTileID id = kTiles[ID].BottomTile;
            SDL_assert(kTiles[id].SpriteType == TileSpriteType1x1);
            MppRendererDraw(
                MppSprite{
                    kTiles[id].Color1,
                    kTiles[id].Color2,
                    kTiles[id].Color3,
                    kTiles[id].Color4,
                    kTiles[id].Color5,
                    GetSprite1x1(id, x, y),
                    kTiles[id].SpriteY,
                    MppTile::kSize,
                },
                x * MppTile::kSize,
                y * MppTile::kSize,
                false,
                MppRendererLayerBottomTile);
        }
        MppRendererDraw(
            MppSprite{
                kTiles[ID].Color1,
                kTiles[ID].Color2,
                kTiles[ID].Color3,
                kTiles[ID].Color4,
                kTiles[ID].Color5,
                GetSprite2x2(ID, x, y),
                kTiles[ID].SpriteY,
                MppTile::kSize,
            },
            x * MppTile::kSize,
            y * MppTile::kSize,
            false,
            MppRendererLayerTile);
    }
    else if (kTiles[ID].SpriteType == TileSpriteType1x2_2x1)
    {
        SDL_assert(kTiles[ID].BottomTile == MppTileIDCount);
        MppRendererDraw(
            MppSprite{
                kTiles[ID].Color1,
                kTiles[ID].Color2,
                kTiles[ID].Color3,
                kTiles[ID].Color4,
                kTiles[ID].Color5,
                GetSprite1x1(ID, x, y),
                kTiles[ID].SpriteY,
                MppTile::kSize,
            },
            x * MppTile::kSize,
            y * MppTile::kSize,
            false,
            MppRendererLayerTile);
        MppRendererDraw(
            MppSprite{
                kTiles[ID].Color1,
                kTiles[ID].Color2,
                kTiles[ID].Color3,
                kTiles[ID].Color4,
                kTiles[ID].Color5,
                GetSprite1x2_2x1(ID, x, y),
                kTiles[ID].TopSpriteY,
                MppTile::kSize,
            },
            x * MppTile::kSize,
            y * MppTile::kSize,
            false,
            MppRendererLayerTopTile);
    }
    else
    {
        MppAssert(false);
    }
}

void MppTile::OnAction(MppEntity& instigator)
{
}

void MppTile::OnCollision(MppEntity& instigator)
{
}

MppTileID MppTile::GetID() const
{
    return ID;
}

MppTilePhysicsType MppTile::GetPhysicsType() const
{
    return kTiles[ID].PhysicsType;
}

int MppTile::GetPhysicsX(int x) const
{
    return x * kSize + kTiles[ID].PhysicsOffsetX;
}

int MppTile::GetPhysicsY(int y) const
{
    return y * kSize + kTiles[ID].PhysicsOffsetY;
}

int MppTile::GetPhysicsWidth() const
{
    return kTiles[ID].PhysicsWidth;
}

int MppTile::GetPhysicsHeight() const
{
    return kTiles[ID].PhysicsHeight;
}

bool MppTile::operator==(const MppTile& other) const
{
    return ID == other.ID;
}

bool MppTile::IsValid() const
{
    return ID != MppTileIDInvalid;
}
