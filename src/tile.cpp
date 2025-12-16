#include <SDL3/SDL.h>
#include <savepoint.hpp>

#include "color.hpp"
#include "console.hpp"
#include "level.hpp"
#include "physics.hpp"
#include "renderer.hpp"
#include "sprite.hpp"
#include "tile.hpp"

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
    MppTileType BottomTile;
    int Color1;
    int Color2;
    int Color3;
    int Color4;
    int Color5;
    MppPhysicsType PhysicsType;
    int PhysicsOffsetX;
    int PhysicsOffsetY;
    int PhysicsWidth;
    int PhysicsHeight;
}
static constexpr kTiles[MppTileTypeCount] =
{
    /* grass */
    {
        .SpriteType = TileSpriteType1x1,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .BottomTile = MppTileTypeCount,
        .Color1 = 62,
        .Color2 = 82,
        .Color3 = 40,
        .Color4 = kMppColorDirtBrown,
        .Color5 = 0,
        .PhysicsType = MppPhysicsTypeGroundTile,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kWidth,
        .PhysicsHeight = MppTile::kWidth,
    },
    /* dirt */
    {
        .SpriteType = TileSpriteType1x1,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .BottomTile = MppTileTypeCount,
        .Color1 = kMppColorDirtBrown,
        .Color2 = kMppColorDirtBrown,
        .Color3 = kMppColorDirtBrown,
        .Color4 = kMppColorDirtBrown,
        .Color5 = 0,
        .PhysicsType = MppPhysicsTypeGroundTile,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kWidth,
        .PhysicsHeight = MppTile::kWidth,
    },
    /* stone */
    {
        .SpriteType = TileSpriteType1x2_2x1,
        .SpriteY = 3,
        .TopSpriteY = 4,
        .BottomTile = MppTileTypeCount,
        .Color1 = 444,
        .Color2 = 111,
        .Color3 = 555,
        .Color4 = 0,
        .Color5 = kMppColorDirtBrown,
        .PhysicsType = MppPhysicsTypeWallTile,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kWidth,
        .PhysicsHeight = MppTile::kWidth,
    },
    /* tree */
    {
        .SpriteType = TileSpriteType2x2,
        .SpriteY = 5,
        .TopSpriteY = 0,
        .BottomTile = MppTileTypeGrass,
        .Color1 = 40,
        .Color2 = 82,
        .Color3 = 960,
        .Color4 = 640,
        .Color5 = 494,
        .PhysicsType = MppPhysicsTypeWallTile,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kWidth,
        .PhysicsHeight = MppTile::kWidth,
    },
    /* sand */
    {
        .SpriteType = TileSpriteType1x1,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .BottomTile = MppTileTypeCount,
        .Color1 = 990,
        .Color2 = 990,
        .Color3 = 990,
        .Color4 = kMppColorDirtBrown,
        .Color5 = 0,
        .PhysicsType = MppPhysicsTypeGroundTile,
        .PhysicsOffsetX = 0,
        .PhysicsOffsetY = 0,
        .PhysicsWidth = MppTile::kWidth,
        .PhysicsHeight = MppTile::kWidth,
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

static bool Equals1x1(MppTileType lhs, MppTileType rhs)
{
    return lhs == rhs || kTiles[lhs].BottomTile == rhs || lhs == kTiles[rhs].BottomTile;
}

static int GetSprite1x1(MppTileType lhs, const MppLevel& level, int x, int y)
{
    int hash = 0;
    hash |= Equals1x1(lhs, level.GetTile(x + 0, y - 1).GetType()) << 0;
    hash |= Equals1x1(lhs, level.GetTile(x + 0, y + 1).GetType()) << 1;
    hash |= Equals1x1(lhs, level.GetTile(x + 1, y + 0).GetType()) << 2;
    hash |= Equals1x1(lhs, level.GetTile(x - 1, y + 0).GetType()) << 3;
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

static bool Equals2x2(MppTileType lhs, MppTileType rhs)
{
    return lhs == rhs;
}

static int GetSprite2x2(MppTileType lhs, const MppLevel& level, int x, int y)
{
    int hash = 0;
    hash |=
        (Equals2x2(lhs, level.GetTile(x - 1, y + 0).GetType()) &&
         Equals2x2(lhs, level.GetTile(x + 0, y - 1).GetType()) &&
         Equals2x2(lhs, level.GetTile(x - 1, y - 1).GetType())) << 0;
    hash |=
        (Equals2x2(lhs, level.GetTile(x - 1, y + 0).GetType()) &&
         Equals2x2(lhs, level.GetTile(x + 0, y + 1).GetType()) &&
         Equals2x2(lhs, level.GetTile(x - 1, y + 1).GetType())) << 1;
    hash |=
        (Equals2x2(lhs, level.GetTile(x + 1, y + 0).GetType()) &&
         Equals2x2(lhs, level.GetTile(x + 0, y + 1).GetType()) &&
         Equals2x2(lhs, level.GetTile(x + 1, y + 1).GetType())) << 2;
    hash |=
        (Equals2x2(lhs, level.GetTile(x + 1, y + 0).GetType()) &&
         Equals2x2(lhs, level.GetTile(x + 0, y - 1).GetType()) &&
         Equals2x2(lhs, level.GetTile(x + 1, y - 1).GetType())) << 3;
    return kSprites2x2[hash];
}

static int GetSprite1x2_2x1(MppTileType lhs, const MppLevel& level, int x, int y)
{
    int hash = 0;
    hash |=
        (Equals2x2(lhs, level.GetTile(x - 1, y + 0).GetType()) &&
         Equals2x2(lhs, level.GetTile(x + 0, y - 1).GetType()) &&
        !Equals2x2(lhs, level.GetTile(x - 1, y - 1).GetType())) << 0;
    hash |=
        (Equals2x2(lhs, level.GetTile(x - 1, y + 0).GetType()) &&
         Equals2x2(lhs, level.GetTile(x + 0, y + 1).GetType()) &&
        !Equals2x2(lhs, level.GetTile(x - 1, y + 1).GetType())) << 1;
    hash |=
        (Equals2x2(lhs, level.GetTile(x + 1, y + 0).GetType()) &&
         Equals2x2(lhs, level.GetTile(x + 0, y + 1).GetType()) &&
        !Equals2x2(lhs, level.GetTile(x + 1, y + 1).GetType())) << 2;
    hash |=
        (Equals2x2(lhs, level.GetTile(x + 1, y + 0).GetType()) &&
         Equals2x2(lhs, level.GetTile(x + 0, y - 1).GetType()) &&
        !Equals2x2(lhs, level.GetTile(x + 1, y - 1).GetType())) << 3;
    return kSprites2x2[hash];
}

MppTile::MppTile(MppTileType type)
    : Type{type}
{
}

void MppTile::Update(MppLevel& level, MppRenderer& renderer, int x, int y, float ticks)
{
    SDL_assert(IsValid());
    Render(level, renderer, x, y);
    RenderPhysics(renderer, x, y);
}

void MppTile::Visit(SavepointVisitor& visitor)
{
    if (visitor.IsWriter())
    {
        SDL_assert(IsValid());
    }
    visitor(Type);
}

MppTileType MppTile::GetType() const
{
    return Type;
}

MppPhysicsType MppTile::GetPhysicsType() const
{
    return kTiles[Type].PhysicsType;
}

int MppTile::GetPhysicsX(int x) const
{
    return x * MppTile::kWidth + kTiles[Type].PhysicsOffsetX;
}

int MppTile::GetPhysicsY(int y) const
{
    return y * MppTile::kWidth + kTiles[Type].PhysicsOffsetY;
}

int MppTile::GetPhysicsWidth() const
{
    return kTiles[Type].PhysicsWidth;
}

int MppTile::GetPhysicsHeight() const
{
    return kTiles[Type].PhysicsHeight;
}

bool MppTile::operator==(const MppTile& other) const
{
    return Type == other.Type;
}

bool MppTile::IsValid() const
{
    return !operator==(kMppTileInvalid);
}

void MppTile::Render(const MppLevel& level, MppRenderer& renderer, int x, int y)
{
    if (kTiles[Type].SpriteType == TileSpriteType1x1)
    {
        renderer.Draw(
            MppSprite{
                kTiles[Type].Color1,
                kTiles[Type].Color2,
                kTiles[Type].Color3,
                kTiles[Type].Color4,
                kTiles[Type].Color5,
                GetSprite1x1(Type, level, x, y),
                kTiles[Type].SpriteY,
                MppTile::kWidth,
            },
            x * MppTile::kWidth,
            y * MppTile::kWidth,
            false,
            MppRenderer::LayerTile);
    }
    else if (kTiles[Type].SpriteType == TileSpriteType2x2)
    {
        SDL_assert(kTiles[Type].TopSpriteY == 0);
        if (kTiles[Type].BottomTile != MppTileTypeCount)
        {
            MppTileType type = kTiles[Type].BottomTile;
            SDL_assert(kTiles[type].SpriteType == TileSpriteType1x1);
            renderer.Draw(
                MppSprite{
                    kTiles[type].Color1,
                    kTiles[type].Color2,
                    kTiles[type].Color3,
                    kTiles[type].Color4,
                    kTiles[type].Color5,
                    GetSprite1x1(type, level, x, y),
                    kTiles[type].SpriteY,
                    MppTile::kWidth,
                },
                x * MppTile::kWidth,
                y * MppTile::kWidth,
                false,
                MppRenderer::LayerBottomTile);
        }
        renderer.Draw(
            MppSprite{
                kTiles[Type].Color1,
                kTiles[Type].Color2,
                kTiles[Type].Color3,
                kTiles[Type].Color4,
                kTiles[Type].Color5,
                GetSprite2x2(Type, level, x, y),
                kTiles[Type].SpriteY,
                MppTile::kWidth,
            },
            x * MppTile::kWidth,
            y * MppTile::kWidth,
            false,
            MppRenderer::LayerTile);
    }
    else if (kTiles[Type].SpriteType == TileSpriteType1x2_2x1)
    {
        SDL_assert(kTiles[Type].BottomTile == MppTileTypeCount);
        renderer.Draw(
            MppSprite{
                kTiles[Type].Color1,
                kTiles[Type].Color2,
                kTiles[Type].Color3,
                kTiles[Type].Color4,
                kTiles[Type].Color5,
                GetSprite1x1(Type, level, x, y),
                kTiles[Type].SpriteY,
                MppTile::kWidth,
            },
            x * MppTile::kWidth,
            y * MppTile::kWidth,
            false,
            MppRenderer::LayerTile);
        renderer.Draw(
            MppSprite{
                kTiles[Type].Color1,
                kTiles[Type].Color2,
                kTiles[Type].Color3,
                kTiles[Type].Color4,
                kTiles[Type].Color5,
                GetSprite1x2_2x1(Type, level, x, y),
                kTiles[Type].TopSpriteY,
                MppTile::kWidth,
            },
            x * MppTile::kWidth,
            y * MppTile::kWidth,
            false,
            MppRenderer::LayerTopTile);
    }
    else
    {
        SDL_assert(false);
    }
}

void MppTile::RenderPhysics(MppRenderer& renderer, int x, int y)
{
    if (!MppConsoleDebugPhysics)
    {
        return;
    }
    int color = 0;
    if (GetPhysicsType() & MppPhysicsTypeGroundTile)
    {
        color = kMppColorGroundTilePhysics;
    }
    else if (GetPhysicsType() & MppPhysicsTypeWallTile)
    {
        color = kMppColorWallTilePhysics;
    }
    else if (GetPhysicsType() & MppPhysicsTypeLiquidTile)
    {
        color = kMppColorLiquidTilePhysics;
    }
    x = GetPhysicsX(x);
    y = GetPhysicsY(y);
    float width = GetPhysicsWidth();
    float height = GetPhysicsHeight();
    renderer.DrawRect(color, x, y, width, height, MppRenderer::LayerPhysics);
}