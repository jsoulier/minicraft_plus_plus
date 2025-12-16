#include <cmath>

#include "camera.hpp"
#include "console.hpp"
#include "level.hpp"
#include "tile.hpp"

MppCamera::MppCamera()
    : X{0}
    , Y{0}
    , Width{0}
    , Height{0}
    , TileX1{0}
    , TileX2{0}
    , TileY1{0}
    , TileY2{0}
{
}

void MppCamera::SetPosition(int x, int y, int size)
{
    X = x - Width / 2 + size / 2;
    Y = y - Height / 2 + size / 2;
}

void MppCamera::SetSize(int width, int height)
{
    Width = width;
    Height = height;
}

void MppCamera::Update(int dt)
{
    int min = 0;
    int max = MppLevel::kWidth - 1;
    int offset = MppConsoleDebugFrustum;
    TileX1 = std::clamp(X / MppTile::kWidth + offset, min, max);
    TileY1 = std::clamp(Y / MppTile::kWidth + offset, min, max);
    TileX2 = std::clamp((X + Width) / MppTile::kWidth - offset, min, max);
    TileY2 = std::clamp((Y + Height) / MppTile::kWidth - offset, min, max);
}