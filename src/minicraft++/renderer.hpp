#pragma once

#include <cstdint>

#include <minicraft++/sprite.hpp>

enum MppRendererLayer : uint8_t
{
    MppRendererLayerBottomTile,
    MppRendererLayerTile,
    MppRendererLayerTopTile,
    MppRendererLayerEntity,
    MppRendererLayerEntityOverlay,
    MppRendererLayerParticleEntity,
    MppRendererLayerDebugPhysics,
    MppRendererLayerDebugNavigation,
    MppRendererLayerDebugFov,
    MppRendererLayerDebugAction,
    MppRendererLayerMenu,
    MppRendererLayerMenuContent,
    MppRendererLayerCount,
};

bool MppRendererInit();
void MppRendererQuit();
void MppRendererSubmit();
void MppRendererMove(int x, int y, int size);
void MppRendererDraw(MppSprite sprite, int x, int y, bool flip, MppRendererLayer layer);
void MppRendererDrawRect(int color, int x, int y, int width, int height, MppRendererLayer layer);
void MppRendererDrawLine(int color, int x1, int y1, int x2, int y2, MppRendererLayer layer);
int MppRendererGetTileX1();
int MppRendererGetTileY1();
int MppRendererGetTileX2();
int MppRendererGetTileY2();
