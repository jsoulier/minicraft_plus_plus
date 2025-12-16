#pragma once

struct MppCamera
{
    MppCamera();
    void SetPosition(int x, int y, int size);
    void SetSize(int width, int height);
    void Update(int dt);

    int X;
    int Y;
    int Width;
    int Height;
    int TileX1;
    int TileX2;
    int TileY1;
    int TileY2;
};