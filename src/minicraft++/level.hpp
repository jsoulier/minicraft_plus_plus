#pragma once

#include <savepoint/savepoint.hpp>
#include <minicraft++/tile.hpp>

#include <cstdint>
#include <string_view>
#include <iostream>
#include <map>

using namespace std;

enum BiomeID : uint16_t
{
    Forest,
    Ocean,
    Snow,
    Desert,
    Volcanic
};

struct {
    BiomeID id;
    map<enum MppTileID,float> tile_percent;
    //enum MppTileID test = MppTileIDGrass;

} Biome;

struct {
    ClusterID;
    int num_tiles;
} LevelCluster;

//map<struct Biome, map<enum MppTileID,float>> biomes;

class MppLevel {
    public:
        //std::array<std::array<MppTile, kSize>, kSize> Tiles;
        //std::vector<std::shared_ptr<MppEntity>> Entities;
        //struct Biome;
    private:
        int LevelID;
};

class Surface: public MppLevel {

};

class Underground1: public MppLevel {

};

class Underground2: public MppLevel {

};

class Underground3: public MppLevel {

};