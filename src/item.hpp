#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>
#include <string_view>
#include <unordered_map>

#include "menu.hpp"

class MppFurnitureEntity;
class MppInventory;
class MppItemEntity;
enum MppRendererLayer : uint8_t;
struct MppItemRecipeData;

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
    MppItemIDAnvil,
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
    MppItemTypeConsumable,
};

class MppItemRecipe : public MppMenu
{
public:
    MppItemRecipe(const MppItemRecipeData& data);
    bool CanCraft(MppItemID id) const;
    bool CanCraft(MppItemID id, const std::shared_ptr<MppInventory>& inventory) const;
    void Craft(const std::shared_ptr<MppInventory>& inventory) const;
    void Render() const;

private:
    const MppItemRecipeData& Data;
};

class MppItem
{
public:
    static constexpr int kSize = 8;

    MppItem();
    MppItem(MppItemID id);
    void Visit(SavepointVisitor& visitor);
    void Render(int x, int y, MppRendererLayer layer) const;
    const std::string_view& GetName() const;
    std::shared_ptr<MppItemEntity> CreateItemEntity() const;
    std::shared_ptr<MppFurnitureEntity> CreateFurnitureEntity() const;
    MppItemRecipe GetRecipe() const;
    void Add(int count = 1);
    MppItem Remove(int count = 1);
    int GetCount() const;
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
