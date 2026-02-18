#pragma once

#include <cstdint>

#include <minicraft++/sprite.hpp>

enum MppRendererLayer : uint8_t
{
    MppRendererLayerBottomTile,
    MppRendererLayerTile,
    MppRendererLayerTopTile,

    MppRendererLayerEntity,
    MppRendererLayerTopEntity,
    MppRendererLayerEntity1 = MppRendererLayerEntity,
    MppRendererLayerTopEntity1 = MppRendererLayerTopEntity,
    MppRendererLayerEntity2,
    MppRendererLayerTopEntity2,
    MppRendererLayerEntity3,
    MppRendererLayerTopEntity3,

    MppRendererLayerParticleEntity,
    MppRendererLayerDebugPhysics,
    MppRendererLayerDebugNavigation,
    MppRendererLayerDebugFov,
    MppRendererLayerDebugAction,

    MppRendererLayerMenu,
    MppRendererLayerTopMenu,
    MppRendererLayerMenu1 = MppRendererLayerMenu,
    MppRendererLayerTopMenu1 = MppRendererLayerTopMenu,
    MppRendererLayerMenu2,
    MppRendererLayerTopMenu2,
    MppRendererLayerMenu3,
    MppRendererLayerTopMenu3,
    MppRendererLayerMenu4,
    MppRendererLayerTopMenu4,
    MppRendererLayerMenu5,
    MppRendererLayerTopMenu5,

    MppRendererLayerCount,
    MppRendererLayerMenuBegin = MppRendererLayerMenu,
};

enum MppRendererMod : uint8_t
{
    MppRendererModNone,
    MppRendererModFlipHorizontally,
    MppRendererModFlipVertically,
    MppRendererModRotate90CW,
    MppRendererModRotate90CCW,
};

class MppRendererLayerOverride
{
public:
    MppRendererLayerOverride(MppRendererLayer key, MppRendererLayer value);
    ~MppRendererLayerOverride();

private:
    MppRendererLayer Key;
    MppRendererLayer Value;
};

bool MppRendererInit();
void MppRendererQuit();
void MppRendererSubmit(int lightColor);
void MppRendererMove(int x, int y, int size);
void MppRendererDraw(MppSprite sprite, int x, int y, MppRendererMod mod, MppRendererLayer layer);
void MppRendererDrawRect(int color, int x, int y, int width, int height, MppRendererLayer layer);
void MppRendererDrawLine(int color, int x1, int y1, int x2, int y2, MppRendererLayer layer);
void MppRendererDrawLight(int color, int x, int y, int radius, int strength);
int MppRendererGetTileX1();
int MppRendererGetTileY1();
int MppRendererGetTileX2();
int MppRendererGetTileY2();
