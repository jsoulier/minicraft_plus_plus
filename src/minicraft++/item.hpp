#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>
#include <string_view>
#include <unordered_map>

#include <minicraft++/menu.hpp>

class MppFurnitureEntity;
class MppInventory;
class MppItemEntity;
class MppProjectileEntity;
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
    MppItemIDWoodSword,
    MppItemIDWoodPickaxe,
    MppItemIDWoodAxe,
    MppItemIDWoodShovel,
    MppItemIDWoodHoe,
    MppItemIDWoodBow,
    MppItemIDString,
    MppItemIDStoneSword,
    MppItemIDStonePickaxe,
    MppItemIDStoneAxe,
    MppItemIDStoneShovel,
    MppItemIDStoneHoe,
    MppItemIDIronSword,
    MppItemIDIronPickaxe,
    MppItemIDIronAxe,
    MppItemIDIronShovel,
    MppItemIDIronHoe,
    MppItemIDHands,
    MppItemIDLantern,
    MppItemIDArrow,
    MppItemIDCount,
};

enum MppItemType
{
    MppItemTypeNone,
    MppItemTypePickaxe = 1,
    MppItemTypeAxe = 2,
    MppItemTypeShovel = 4,
    MppItemTypeSword = 8,
    MppItemTypeHoe = 16,
    MppItemTypeHelmet = 32,
    MppItemTypeCuirass = 64,
    MppItemTypeLeggings = 128,
    MppItemTypeBoots = 256,
    MppItemTypeFurniture = 512,
    MppItemTypeConsumable = 1024,
    MppItemTypeHands = 2048,
    MppItemTypeHeld =
        MppItemTypePickaxe |
        MppItemTypeAxe |
        MppItemTypeShovel |
        MppItemTypeSword |
        MppItemTypeHoe,
    MppItemTypeArmor =
        MppItemTypeHelmet |
        MppItemTypeCuirass |
        MppItemTypeLeggings |
        MppItemTypeBoots,
    MppItemTypeEquipment =
        MppItemTypeHeld |
        MppItemTypeArmor,
};

enum MppItemActionType
{
    MppItemActionTypeNone,
    MppItemActionTypeAttackOrInteract,
    MppItemActionTypeProjectile,
};

inline constexpr MppItemType operator|(MppItemType lhs, MppItemType rhs)
{
    return MppItemType(int(lhs) | int(rhs));
}

inline constexpr MppItemType operator&(MppItemType lhs, MppItemType rhs)
{
    return MppItemType(int(lhs) & int(rhs));
}

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
    std::shared_ptr<MppProjectileEntity> CreateProjectileEntity() const;
    MppItemRecipe GetRecipe() const;
    MppItemRecipe GetActionRecipe() const;
    void Add(int count = 1);
    MppItem Remove(int count = 1);
    int GetCount() const;
    MppItemID GetID() const;
    MppItemType GetType() const;
    MppItemActionType GetActionType() const;
    int GetColor1() const;
    int GetColor2() const;
    int GetColor3() const;
    int GetColor4() const;
    int GetColor5() const;
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

static MppItem kMppItemInvalid{MppItemIDInvalid};
static MppItem kMppItemHands{MppItemIDHands};

