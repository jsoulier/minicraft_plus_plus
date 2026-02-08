#pragma once

#include <SDL3/SDL.h>

static constexpr int kMppColorBlack = 0;
static constexpr int kMppColorRed = 500;
static constexpr int kMppColorGreen = 50;
static constexpr int kMppColorBlue = 5;
static constexpr int kMppColorYellow = 550;
static constexpr int kMppColorMagenta = 505;
static constexpr int kMppColorCyan = 55;
static constexpr int kMppColorWhite = 555;

static constexpr int kMppColorDebugNavigation = kMppColorMagenta;
static constexpr int kMppColorDebugFurnitureEntityPhysics = kMppColorCyan;
static constexpr int kMppColorDebugMobEntityPhysics = kMppColorYellow;
static constexpr int kMppColorDebugPlayerEntityPhysics = kMppColorMagenta;
static constexpr int kMppColorDebugGroundTilePhysics = kMppColorMagenta;
static constexpr int kMppColorDebugWallTilePhysics = kMppColorGreen;
static constexpr int kMppColorDebugLiquidTilePhysics = kMppColorBlue;
static constexpr int kMppColorDebugFov = kMppColorRed;
static constexpr int kMppColorDebugAction = kMppColorBlue;

static constexpr int kMppColorAnvil1 = 111;
static constexpr int kMppColorAnvil2 = 222;
static constexpr int kMppColorAnvil3 = 333;
static constexpr int kMppColorAnvil4 = 444;
static constexpr int kMppColorAnvil5 = 555;
static constexpr int kMppColorChest1 = 0;
static constexpr int kMppColorChest2 = 211;
static constexpr int kMppColorChest3 = 322;
static constexpr int kMppColorChest4 = 422;
static constexpr int kMppColorChest5 = 533;
static constexpr int kMppColorFurnace1 = 0;
static constexpr int kMppColorFurnace2 = 111;
static constexpr int kMppColorFurnace3 = 333;
static constexpr int kMppColorFurnace4 = 444;
static constexpr int kMppColorFurnace5 = 540;
static constexpr int kMppColorMenuLocked = 111;
static constexpr int kMppColorMenuUnlocked = 555;
static constexpr int kMppColorWorkbench1 = 0;
static constexpr int kMppColorWorkbench2 = 211;
static constexpr int kMppColorWorkbench3 = 322;
static constexpr int kMppColorWorkbench4 = 433;
static constexpr int kMppColorWorkbench5 = 544;
static constexpr int kMppColorLantern1 = 0;
static constexpr int kMppColorLantern2 = 111;
static constexpr int kMppColorLantern3 = 0;
static constexpr int kMppColorLantern4 = 310;
static constexpr int kMppColorLantern5 = 550;

SDL_Color MppColorGet(int inColor);