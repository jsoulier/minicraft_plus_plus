#include <cmath>

#include "camera.hpp"
#include "level.hpp"
#include "tile.hpp"

MppCamera::MppCamera()
    : X{0.0f}
    , Y{0.0f}
    , Width{0.0f}
    , Height{0.0f}
    , TileX1{0}
    , TileX2{0}
    , TileY1{0}
    , TileY2{0}
{
}

void MppCamera::SetPosition(float x, float y)
{
    X = x - Width / 2;
    Y = y - Height / 2;
}

void MppCamera::SetSize(float width, float height)
{
    Width = width;
    Height = height;
}

void MppCamera::Update(float dt)
{
    TileX1 = std::max(int(X / MppTile::kWidth), 0);
    TileY1 = std::max(int(Y / MppTile::kWidth), 0);
    TileX2 = std::min(int((X + Width) / MppTile::kWidth), MppLevel::kWidth - 1);
    TileY2 = std::min(int((Y + Height) / MppTile::kWidth), MppLevel::kWidth - 1);
}