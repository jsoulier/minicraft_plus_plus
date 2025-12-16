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

void MppCamera::SetPosition(float x, float y, float size)
{
    X = x - Width / 2 + size / 2;
    Y = y - Height / 2 + size / 2;
}

void MppCamera::SetSize(float width, float height)
{
    Width = width;
    Height = height;
}

void MppCamera::Update(float dt)
{
    int min = 0;
    int max = MppLevel::kWidth - 1;
    TileX1 = std::clamp(int(X / MppTile::kWidth), min, max);
    TileY1 = std::clamp(int(Y / MppTile::kWidth), min, max);
    TileX2 = std::clamp(int((X + Width) / MppTile::kWidth) + 1, min, max);
    TileY2 = std::clamp(int((Y + Height) / MppTile::kWidth) + 1, min, max);
}