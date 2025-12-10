#include <SDL3/SDL.h>
#include <savepoint.hpp>

#include "level.hpp"
#include "renderer.hpp"
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
        .Color4 = 840,
        .Color5 = 0,

    },
    /* dirt */
    {
        .SpriteType = TileSpriteType1x1,
        .SpriteY = 0,
        .TopSpriteY = 0,
        .BottomTile = MppTileTypeCount,
        .Color1 = 840,
        .Color2 = 840,
        .Color3 = 840,
        .Color4 = 840,
        .Color5 = 0,

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
        .Color5 = 840,
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
        .Color4 = 840,
        .Color5 = 0,

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

static void Render(MppTileType type, const MppLevel& level, MppRenderer& renderer, int x, int y)
{
    if (kTiles[type].SpriteType == TileSpriteType1x1)
    {
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
            MppRenderer::LayerTile);
    }
    else if (kTiles[type].SpriteType == TileSpriteType2x2)
    {
        SDL_assert(kTiles[type].TopSpriteY == 0);
        if (kTiles[type].BottomTile != MppTileTypeCount)
        {
            MppTileType bottomType = kTiles[type].BottomTile;
            SDL_assert(kTiles[bottomType].SpriteType == TileSpriteType1x1);
            renderer.Draw(
                MppSprite{
                    kTiles[bottomType].Color1,
                    kTiles[bottomType].Color2,
                    kTiles[bottomType].Color3,
                    kTiles[bottomType].Color4,
                    kTiles[bottomType].Color5,
                    GetSprite1x1(bottomType, level, x, y),
                    kTiles[bottomType].SpriteY,
                    MppTile::kWidth,
                },
                x * MppTile::kWidth,
                y * MppTile::kWidth,
                MppRenderer::LayerBottomTile);
        }
        renderer.Draw(
            MppSprite{
                kTiles[type].Color1,
                kTiles[type].Color2,
                kTiles[type].Color3,
                kTiles[type].Color4,
                kTiles[type].Color5,
                GetSprite2x2(type, level, x, y),
                kTiles[type].SpriteY,
                MppTile::kWidth,
            },
            x * MppTile::kWidth,
            y * MppTile::kWidth,
            MppRenderer::LayerTile);
    }
    else if (kTiles[type].SpriteType == TileSpriteType1x2_2x1)
    {
        SDL_assert(kTiles[type].BottomTile == MppTileTypeCount);
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
            MppRenderer::LayerTile);
        renderer.Draw(
            MppSprite{
                kTiles[type].Color1,
                kTiles[type].Color2,
                kTiles[type].Color3,
                kTiles[type].Color4,
                kTiles[type].Color5,
                GetSprite1x2_2x1(type, level, x, y),
                kTiles[type].TopSpriteY,
                MppTile::kWidth,
            },
            x * MppTile::kWidth,
            y * MppTile::kWidth,
            MppRenderer::LayerTopTile);
    }
    else
    {
        SDL_assert(false);
    }
}

MppTile::MppTile(MppTileType type)
    : Type{type}
{
}

void MppTile::Update(MppLevel& level, MppRenderer& renderer, int x, int y, float dt, float ticks)
{
    SDL_assert(IsValid());
    Render(Type, level, renderer, x, y);
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

bool MppTile::operator==(const MppTile& other) const
{
    return Type == other.Type;
}

bool MppTile::IsValid() const
{
    return !operator==(kMppTileInvalid);
}
