#pragma once

struct MppCamera
{
    MppCamera();
    void SetPosition(float x, float y, float size);
    void SetSize(float width, float height);
    void Update(float dt);

    float X;
    float Y;
    float Width;
    float Height;
    int TileX1;
    int TileX2;
    int TileY1;
    int TileY2;
};