#pragma once

#include <savepoint/fwd.hpp>

#include <cstdint>
#include <memory>
#include <string_view>

class MppFurnitureEntity;
class MppItemEntity;
enum MppRendererLayer : uint8_t;

enum MppItemID
{
    MppItemIDInvalid,
    MppItemIDWood,
    MppItemIDApple,
    MppItemIDIronHelmet,
    MppItemIDIronCuirass,
    MppItemIDIronLeggings,
    MppItemIDIronBoots,
    MppItemIDIronOre,
    MppItemIDIronBar,
    MppItemIDStone,
    MppItemIDWorkbench,
    MppItemIDFurnace,
    MppItemIDChest,
    MppItemIDHeart,
    MppItemIDEnergy,
    MppItemIDFood,
    MppItemIDCount,
};

enum MppItemType
{
    MppItemTypeNone,
    MppItemTypePickaxe,
    MppItemTypeAxe,
    MppItemTypeShovel,
    MppItemTypeSword,
    MppItemTypeHoe,
    MppItemTypeHelmet,
    MppItemTypeCuirass,
    MppItemTypeLeggings,
    MppItemTypeBoots,
    MppItemTypeFurniture,
};

class MppItem
{
public:
    static constexpr int kSize = 8;

    MppItem();
    MppItem(MppItemID id);
    void Visit(SavepointVisitor& visitor);
    void Render(int x, int y, MppRendererLayer layer) const;
    std::string_view GetName() const;
    std::shared_ptr<MppItemEntity> CreateItemEntity() const;
    std::shared_ptr<MppFurnitureEntity> CreateFurnitureEntity() const;
    MppItemID GetID() const;
    MppItemType GetType() const;
    int GetPhysicsOffsetX() const;
    int GetPhysicsOffsetY() const;
    int GetPhysicsWidth() const;
    int GetPhysicsHeight() const;
    bool operator==(const MppItem& other) const;
    bool IsValid() const;

private:
    MppItemID ID;
    int Count;
};

static MppItem kMppItemInvalid;
