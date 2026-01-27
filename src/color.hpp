#pragma once

#include <SDL3/SDL.h>

static constexpr int kMppColorBlack = 0;
static constexpr int kMppColorRed = 900;
static constexpr int kMppColorGreen = 90;
static constexpr int kMppColorBlue = 9;
static constexpr int kMppColorYellow = 990;
static constexpr int kMppColorMagenta = 909;
static constexpr int kMppColorCyan = 99;
static constexpr int kMppColorWhite = 999;

static constexpr int kMppColorDebugNavigation = kMppColorMagenta;
static constexpr int kMppColorDebugFurnitureEntityPhysics = kMppColorCyan;
static constexpr int kMppColorDebugMobEntityPhysics = kMppColorYellow;
static constexpr int kMppColorDebugPlayerEntityPhysics = kMppColorMagenta;
static constexpr int kMppColorDebugGroundTilePhysics = kMppColorMagenta;
static constexpr int kMppColorDebugWallTilePhysics = kMppColorGreen;
static constexpr int kMppColorDebugLiquidTilePhysics = kMppColorBlue;

static constexpr int kMppColorAnvil1 = 111;
static constexpr int kMppColorAnvil2 = 222;
static constexpr int kMppColorAnvil3 = 333;
static constexpr int kMppColorAnvil4 = 444;
static constexpr int kMppColorAnvil5 = 555;
static constexpr int kMppColorChest1 = 0;
static constexpr int kMppColorChest2 = 311;
static constexpr int kMppColorChest3 = 533;
static constexpr int kMppColorChest4 = 755;
static constexpr int kMppColorChest5 = 966;
static constexpr int kMppColorDirt = 840;
static constexpr int kMppColorFurnace1 = 0;
static constexpr int kMppColorFurnace2 = 333;
static constexpr int kMppColorFurnace3 = 555;
static constexpr int kMppColorFurnace4 = 777;
static constexpr int kMppColorFurnace5 = 960;
static constexpr int kMppColorIron1 = 0;
static constexpr int kMppColorIron2 = 444;
static constexpr int kMppColorIron3 = 555;
static constexpr int kMppColorIron4 = 666;
static constexpr int kMppColorIron5 = 777;
static constexpr int kMppColorMenuBackground = 9;
static constexpr int kMppColorMenuLockedForeground = 555;
static constexpr int kMppColorMenuUnlockedForeground = 999;
static constexpr int kMppColorPlayerBorder = 0;
static constexpr int kMppColorPlayerPants = 9;
static constexpr int kMppColorPlayerShirt = 900;
static constexpr int kMppColorPlayerShoes = 421;
static constexpr int kMppColorPlayerSkin = 964;
static constexpr int kMppColorSheep1 = 0;
static constexpr int kMppColorSheep2 = 999;
static constexpr int kMppColorSheep3 = 0;
static constexpr int kMppColorSheep4 = 964;
static constexpr int kMppColorSheep5 = 0;
static constexpr int kMppColorStone1 = 444;
static constexpr int kMppColorStone2 = 111;
static constexpr int kMppColorStone3 = 666;
static constexpr int kMppColorStone4 = 555;
static constexpr int kMppColorWood1 = 640;
static constexpr int kMppColorWood2 = 750;
static constexpr int kMppColorWood3 = 860;
static constexpr int kMppColorWorkbench1 = 0;
static constexpr int kMppColorWorkbench2 = 311;
static constexpr int kMppColorWorkbench3 = 533;
static constexpr int kMppColorWorkbench4 = 755;
static constexpr int kMppColorWorkbench5 = 966;
static constexpr int kMppColorZombieBorder = 0;
static constexpr int kMppColorZombiePants = 640;
static constexpr int kMppColorZombieShirt = 640;
static constexpr int kMppColorZombieShoes = 320;
static constexpr int kMppColorZombieSkin = 80;

SDL_Color MppColorGet(int inColor);