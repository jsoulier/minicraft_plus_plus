#pragma once

#include <savepoint/fwd.hpp>

#include <memory>
#include <string_view>
#include <unordered_map>

class MppFurnitureEntity;
class MppInventory;
class MppItemEntity;

enum MppItemID
{
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
    MppItemIDCount,
    MppItemIDInvalid = MppItemIDCount,
};

enum MppItemFlag
{
    MppItemFlagNone = 1,
    MppItemFlagConsumable = 2,
    MppItemFlagHeld = 4,
    MppItemFlagArmor = 8,
    MppItemFlagStackable  = ~(MppItemFlagHeld | MppItemFlagArmor),
};

enum MppItemType
{
    MppItemTypeNone,
    MppItemTypePickaxe,
    MppItemTypeAxe,
    MppItemTypeShovel,
    MppItemTypeSword,
    MppItemTypeHoe,
    MppItemTypeCuirass,
    MppItemTypeHelmet,
    MppItemTypeBoots,
    MppItemTypeLeggings,
    MppItemTypeFurniture,
};

enum MppItemRecipeType
{
    MppItemRecipeTypeHand,
    MppItemRecipeTypeWorkbench,
    MppItemRecipeTypeFurnace,
    MppItemRecipeTypeCount,
};

class MppItemRecipe
{
public:
    MppItemRecipe();
    MppItemRecipe(MppItemRecipeType type, MppItemID item, const std::unordered_map<MppItemID, int>& materials);
    void Craft(std::shared_ptr<MppInventory>& inventory);
    bool IsValid() const;

private:
    MppItemRecipeType Type;
    MppItemID Item;
    std::unordered_map<MppItemID, int> Materials;
};

class MppItem
{
public:
    static constexpr int kWidth = 8;

    MppItem() : ID{MppItemIDInvalid} {}
    MppItem(MppItemID id);
    void Visit(SavepointVisitor& visitor);
    void AddItem();
    void RemoveItem();
    int GetItems() const;
    std::string_view GetName() const;
    MppItemID GetID() const;
    MppItemFlag GetFlag() const;
    MppItemType GetType() const;
    std::shared_ptr<MppItemEntity> CreateItemEntity() const;
    std::shared_ptr<MppFurnitureEntity> CreateFurnitureEntity() const;
    int GetColor1() const;
    int GetColor2() const;
    int GetColor3() const;
    int GetColor4() const;
    int GetColor5() const;
    int GetSpriteX() const;
    int GetSpriteY() const;
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

static const MppItem kMppItemInvalid;