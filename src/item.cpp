#include <SDL3/SDL.h>
#include <savepoint/savepoint.hpp>

#include <memory>
#include <string_view>
#include <unordered_map>

#include "assert.hpp"
#include "color.hpp"
#include "entity/anvil.hpp"
#include "entity/chest.hpp"
#include "entity/furnace.hpp"
#include "entity/item.hpp"
#include "entity/workbench.hpp"
#include "inventory.hpp"
#include "item.hpp"
#include "renderer.hpp"

static constexpr int kIronColor1 = 0;
static constexpr int kIronColor2 = 222;
static constexpr int kIronColor3 = 333;
static constexpr int kIronColor4 = 444;
static constexpr int kIronColor5 = 555;
static constexpr int kStoneColor1 = 333;
static constexpr int kStoneColor2 = 111;
static constexpr int kStoneColor3 = 555;
static constexpr int kStoneColor4 = 444;
static constexpr int kStoneColor5 = 444;
static constexpr int kWoodColor1 = 320;
static constexpr int kWoodColor2 = 430;
static constexpr int kWoodColor3 = 540;
static constexpr int kWoodColor4 = 540;
static constexpr int kWoodColor5 = 540;

using CreateFurnitureEntityFunction = std::shared_ptr<MppFurnitureEntity>(*)();

static std::shared_ptr<MppFurnitureEntity> CreateWorkbenchEntity()
{
    return std::make_shared<MppWorkbenchEntity>();
}

static std::shared_ptr<MppFurnitureEntity> CreateFurnaceEntity()
{
    return std::make_shared<MppFurnaceEntity>();
}

static std::shared_ptr<MppFurnitureEntity> CreateChestEntity()
{
    return std::make_shared<MppChestEntity>();
}

static std::shared_ptr<MppFurnitureEntity> CreateAnvilEntity()
{
    return std::make_shared<MppAnvilEntity>();
}

struct MppItemRecipeData
{
    MppItemID ID;
    std::unordered_map<MppItemID, int> Items;
};

struct
{
    std::string_view Name;
    MppItemType Type;
    MppItemRecipeData Recipe;
    CreateFurnitureEntityFunction CreateFurnitureEntity;
    int Color1;
    int Color2;
    int Color3;
    int Color4;
    int Color5;
    int SpriteX;
    int SpriteY;
}
static const kItems[MppItemIDCount] =
{
    {
        .Name = "invalid",
        .Type = MppItemTypeConsumable,
        .Color1 = 0,
        .Color2 = 0,
        .Color3 = 0,
        .Color4 = 0,
        .Color5 = 0,
        .SpriteX = 0,
        .SpriteY = 0,
    },
    {
        .Name = "wood",
        .Type = MppItemTypeConsumable,
        .Color1 = kWoodColor1,
        .Color2 = kWoodColor2,
        .Color3 = kWoodColor3,
        .Color4 = kWoodColor4,
        .Color5 = kWoodColor5,
        .SpriteX = 3,
        .SpriteY = 12,
    },
    {
        .Name = "apple",
        .Type = MppItemTypeConsumable,
        .Color1 = 500,
        .Color2 = 700,
        .Color3 = 900,
        .Color4 = 740,
        .Color5 = 90,
        .SpriteX = 1,
        .SpriteY = 12,
    },
    {
        .Name = "iron helmet",
        .Type = MppItemTypeHelmet,
        .Recipe = {MppItemIDAnvil, {{MppItemIDIronBar, 4}}},
        .Color1 = kIronColor1,
        .Color2 = kIronColor2,
        .Color3 = kIronColor3,
        .Color4 = kIronColor4,
        .Color5 = kIronColor5,
        .SpriteX = 0,
        .SpriteY = 13,
    },
    {
        .Name = "iron cuirass",
        .Type = MppItemTypeCuirass,
        .Recipe = {MppItemIDAnvil, {{MppItemIDIronBar, 8}}},
        .Color1 = kIronColor1,
        .Color2 = kIronColor2,
        .Color3 = kIronColor3,
        .Color4 = kIronColor4,
        .Color5 = kIronColor5,
        .SpriteX = 1,
        .SpriteY = 13,
    },
    {
        .Name = "iron leggings",
        .Type = MppItemTypeLeggings,
        .Recipe = {MppItemIDAnvil, {{MppItemIDIronBar, 7}}},
        .Color1 = kIronColor1,
        .Color2 = kIronColor2,
        .Color3 = kIronColor3,
        .Color4 = kIronColor4,
        .Color5 = kIronColor5,
        .SpriteX = 2,
        .SpriteY = 13,
    },
    {
        .Name = "iron boots",
        .Type = MppItemTypeBoots,
        .Recipe = {MppItemIDAnvil, {{MppItemIDIronBar, 4}}},
        .Color1 = kIronColor1,
        .Color2 = kIronColor2,
        .Color3 = kIronColor3,
        .Color4 = kIronColor4,
        .Color5 = kIronColor5,
        .SpriteX = 3,
        .SpriteY = 13,
    },
    {
        .Name = "iron ore",
        .Type = MppItemTypeConsumable,
        .Color1 = kIronColor1,
        .Color2 = kIronColor2,
        .Color3 = kIronColor3,
        .Color4 = kIronColor4,
        .Color5 = kIronColor5,
        .SpriteX = 2,
        .SpriteY = 12,
    },
    {
        .Name = "iron bar",
        .Type = MppItemTypeConsumable,
        .Recipe = {MppItemIDFurnace, {{MppItemIDIronOre, 1}}},
        .Color1 = kIronColor1,
        .Color2 = kIronColor2,
        .Color3 = kIronColor3,
        .Color4 = kIronColor4,
        .Color5 = kIronColor5,
        .SpriteX = 0,
        .SpriteY = 12,
    },
    {
        .Name = "stone",
        .Type = MppItemTypeConsumable,
        .Color1 = kStoneColor1,
        .Color2 = kStoneColor2,
        .Color3 = kStoneColor3,
        .Color4 = kStoneColor4,
        .Color5 = kStoneColor5,
        .SpriteX = 2,
        .SpriteY = 12,
    },
    {
        .Name = "workbench",
        .Type = MppItemTypeFurniture,
        .CreateFurnitureEntity = CreateWorkbenchEntity,
        .Color1 = kMppColorWorkbench1,
        .Color2 = kMppColorWorkbench2,
        .Color3 = kMppColorWorkbench3,
        .Color4 = kMppColorWorkbench4,
        .Color5 = kMppColorWorkbench5,
        .SpriteX = 0,
        .SpriteY = 14,
    },
    {
        .Name = "furnace",
        .Type = MppItemTypeFurniture,
        .CreateFurnitureEntity = CreateFurnaceEntity,
        .Color1 = kMppColorFurnace1,
        .Color2 = kMppColorFurnace2,
        .Color3 = kMppColorFurnace3,
        .Color4 = kMppColorFurnace4,
        .Color5 = kMppColorFurnace5,
        .SpriteX = 1,
        .SpriteY = 14,
    },
    {
        .Name = "chest",
        .Type = MppItemTypeFurniture,
        .CreateFurnitureEntity = CreateChestEntity,
        .Color1 = kMppColorChest1,
        .Color2 = kMppColorChest2,
        .Color3 = kMppColorChest3,
        .Color4 = kMppColorChest4,
        .Color5 = kMppColorChest5,
        .SpriteX = 2,
        .SpriteY = 14,
    },
    {
        .Name = "heart",
        .Type = MppItemTypeNone,
        .Color1 = 000,
        .Color2 = 300,
        .Color3 = 400,
        .Color4 = 500,
        .Color5 = 0,
        .SpriteX = 0,
        .SpriteY = 16,
    },
    {
        .Name = "energy",
        .Type = MppItemTypeNone,
        .Color1 = 0,
        .Color2 = 550,
        .Color3 = 330,
        .Color4 = 0,
        .Color5 = 0,
        .SpriteX = 1,
        .SpriteY = 16,
    },
    {
        .Name = "food",
        .Type = MppItemTypeNone,
        .Color1 = 0,
        .Color2 = 310,
        .Color3 = 555,
        .Color4 = 530,
        .Color5 = 420,
        .SpriteX = 2,
        .SpriteY = 16,
    },
    {
        .Name = "anvil",
        .Type = MppItemTypeNone,
        .CreateFurnitureEntity = CreateAnvilEntity,
        .Color1 = kMppColorAnvil1,
        .Color2 = kMppColorAnvil2,
        .Color3 = kMppColorAnvil3,
        .Color4 = kMppColorAnvil4,
        .Color5 = kMppColorAnvil5,
        .SpriteX = 3,
        .SpriteY = 14,
    },
    {
        .Name = "wood sword",
        .Type = MppItemTypeSword,
        .Recipe = {MppItemIDWorkbench, {{MppItemIDWood, 3}}},
        .Color1 = kWoodColor1,
        .Color2 = kWoodColor2,
        .Color3 = kWoodColor3,
        .Color4 = kWoodColor4,
        .Color5 = kWoodColor5,
        .SpriteX = 4,
        .SpriteY = 13,
    },
    {
        .Name = "wood pickaxe",
        .Type = MppItemTypePickaxe,
        .Recipe = {MppItemIDWorkbench, {{MppItemIDWood, 5}}},
        .Color1 = kWoodColor1,
        .Color2 = kWoodColor2,
        .Color3 = kWoodColor3,
        .Color4 = kWoodColor4,
        .Color5 = kWoodColor5,
        .SpriteX = 5,
        .SpriteY = 13,
    },
    {
        .Name = "wood axe",
        .Type = MppItemTypeAxe,
        .Recipe = {MppItemIDWorkbench, {{MppItemIDWood, 4}}},
        .Color1 = kWoodColor1,
        .Color2 = kWoodColor2,
        .Color3 = kWoodColor3,
        .Color4 = kWoodColor4,
        .Color5 = kWoodColor5,
        .SpriteX = 6,
        .SpriteY = 13,
    },
    {
        .Name = "wood shovel",
        .Type = MppItemTypeShovel,
        .Recipe = {MppItemIDWorkbench, {{MppItemIDWood, 3}}},
        .Color1 = kWoodColor1,
        .Color2 = kWoodColor2,
        .Color3 = kWoodColor3,
        .Color4 = kWoodColor4,
        .Color5 = kWoodColor5,
        .SpriteX = 7,
        .SpriteY = 13,
    },
    {
        .Name = "wood hoe",
        .Type = MppItemTypeHoe,
        .Recipe = {MppItemIDWorkbench, {{MppItemIDWood, 4}}},
        .Color1 = kWoodColor1,
        .Color2 = kWoodColor2,
        .Color3 = kWoodColor3,
        .Color4 = kWoodColor4,
        .Color5 = kWoodColor5,
        .SpriteX = 8,
        .SpriteY = 13,
    },
    {
        .Name = "wood bow",
        .Type = MppItemTypeBow,
        .Recipe = {MppItemIDWorkbench, {{MppItemIDWood, 3}, {MppItemIDString, 3}}},
        .Color1 = kWoodColor1,
        .Color2 = kWoodColor2,
        .Color3 = kWoodColor3,
        .Color4 = kWoodColor4,
        .Color5 = kWoodColor5,
        .SpriteX = 9,
        .SpriteY = 13,
    },
    {
        .Name = "string",
        .Type = MppItemTypeConsumable,
        .Color1 = 0,
        .Color2 = 0,
        .Color3 = 0,
        .Color4 = 0,
        .Color5 = 0,
        .SpriteX = 0,
        .SpriteY = 0,
    },
    {
        .Name = "stone sword",
        .Type = MppItemTypeSword,
        .Recipe = {MppItemIDWorkbench, {{MppItemIDStone, 3}}},
        .Color1 = kStoneColor1,
        .Color2 = kStoneColor2,
        .Color3 = kStoneColor3,
        .Color4 = kStoneColor4,
        .Color5 = kStoneColor5,
        .SpriteX = 4,
        .SpriteY = 13,
    },
    {
        .Name = "stone pickaxe",
        .Type = MppItemTypePickaxe,
        .Recipe = {MppItemIDWorkbench, {{MppItemIDStone, 5}}},
        .Color1 = kStoneColor1,
        .Color2 = kStoneColor2,
        .Color3 = kStoneColor3,
        .Color4 = kStoneColor4,
        .Color5 = kStoneColor5,
        .SpriteX = 5,
        .SpriteY = 13,
    },
    {
        .Name = "stone axe",
        .Type = MppItemTypeAxe,
        .Recipe = {MppItemIDWorkbench, {{MppItemIDStone, 4}}},
        .Color1 = kStoneColor1,
        .Color2 = kStoneColor2,
        .Color3 = kStoneColor3,
        .Color4 = kStoneColor4,
        .Color5 = kStoneColor5,
        .SpriteX = 6,
        .SpriteY = 13,
    },
    {
        .Name = "stone shovel",
        .Type = MppItemTypeShovel,
        .Recipe = {MppItemIDWorkbench, {{MppItemIDStone, 3}}},
        .Color1 = kStoneColor1,
        .Color2 = kStoneColor2,
        .Color3 = kStoneColor3,
        .Color4 = kStoneColor4,
        .Color5 = kStoneColor5,
        .SpriteX = 7,
        .SpriteY = 13,
    },
    {
        .Name = "stone hoe",
        .Type = MppItemTypeHoe,
        .Recipe = {MppItemIDWorkbench, {{MppItemIDStone, 4}}},
        .Color1 = kStoneColor1,
        .Color2 = kStoneColor2,
        .Color3 = kStoneColor3,
        .Color4 = kStoneColor4,
        .Color5 = kStoneColor5,
        .SpriteX = 8,
        .SpriteY = 13,
    },
    {
        .Name = "iron sword",
        .Type = MppItemTypeSword,
        .Recipe = {MppItemIDAnvil, {{MppItemIDIronBar, 3}}},
        .Color1 = kIronColor1,
        .Color2 = kIronColor2,
        .Color3 = kIronColor3,
        .Color4 = kIronColor4,
        .Color5 = kIronColor5,
        .SpriteX = 4,
        .SpriteY = 13,
    },
    {
        .Name = "iron pickaxe",
        .Type = MppItemTypePickaxe,
        .Recipe = {MppItemIDAnvil, {{MppItemIDIronBar, 5}}},
        .Color1 = kIronColor1,
        .Color2 = kIronColor2,
        .Color3 = kIronColor3,
        .Color4 = kIronColor4,
        .Color5 = kIronColor5,
        .SpriteX = 5,
        .SpriteY = 13,
    },
    {
        .Name = "iron axe",
        .Type = MppItemTypeAxe,
        .Recipe = {MppItemIDAnvil, {{MppItemIDIronBar, 4}}},
        .Color1 = kIronColor1,
        .Color2 = kIronColor2,
        .Color3 = kIronColor3,
        .Color4 = kIronColor4,
        .Color5 = kIronColor5,
        .SpriteX = 6,
        .SpriteY = 13,
    },
    {
        .Name = "iron shovel",
        .Type = MppItemTypeShovel,
        .Recipe = {MppItemIDAnvil, {{MppItemIDIronBar, 3}}},
        .Color1 = kIronColor1,
        .Color2 = kIronColor2,
        .Color3 = kIronColor3,
        .Color4 = kIronColor4,
        .Color5 = kIronColor5,
        .SpriteX = 7,
        .SpriteY = 13,
    },
    {
        .Name = "iron hoe",
        .Type = MppItemTypeHoe,
        .Recipe = {MppItemIDAnvil, {{MppItemIDIronBar, 4}}},
        .Color1 = kIronColor1,
        .Color2 = kIronColor2,
        .Color3 = kIronColor3,
        .Color4 = kIronColor4,
        .Color5 = kIronColor5,
        .SpriteX = 8,
        .SpriteY = 13,
    },
};

MppItemRecipe::MppItemRecipe(const MppItemRecipeData& data)
    : Data{data}
{
    for (auto& [id, count] : Data.Items)
    {
        MppItem item{id};
        // Assumption is made in crafting that only stackables are allowed
        MppAssert(item.GetType() == MppItemTypeConsumable);
    }
    SetX1(130);
    SetY1(4);
    SetX2(246);
    SetY2(64);
}

bool MppItemRecipe::CanCraft(MppItemID id) const
{
    return Data.ID == id;
}

bool MppItemRecipe::CanCraft(MppItemID inID, const std::shared_ptr<MppInventory>& inventory) const
{
    MppAssert(CanCraft(inID));
    MppAssert(inID != MppItemIDInvalid);
    for (auto& [id, count] : Data.Items)
    {
        const MppItem& item = inventory->GetByID(id);
        if (!item.IsValid())
        {
            return false;
        }
        if (item.GetCount() < count)
        {
            return false;
        }
    }
    return true;
}

void MppItemRecipe::Craft(const std::shared_ptr<MppInventory>& inventory) const
{
    MppAssert(CanCraft(Data.ID, inventory));
    for (auto& [id, count] : Data.Items)
    {
        inventory->RemoveByID(id, count);
    }
}

void MppItemRecipe::Render() const
{
    MppMenu::Render();
}

MppItem::MppItem()
    : ID{MppItemIDInvalid}
    , Count{0}
{
}

MppItem::MppItem(MppItemID id)
    : ID{id}
    , Count{1}
{
}

void MppItem::Visit(SavepointVisitor& visitor)
{
    if (visitor.IsWriting())
    {
        MppAssert(IsValid());
    }
    visitor(ID);
    visitor(Count);
}

void MppItem::Render(int x, int y, MppRendererLayer layer) const
{
    MppRendererDraw(MppSprite{
            kItems[ID].Color1,
            kItems[ID].Color2,
            kItems[ID].Color3,
            kItems[ID].Color4,
            kItems[ID].Color5,
            kItems[ID].SpriteX,
            kItems[ID].SpriteY,
            kSize,
        },
        x,
        y,
        false,
        layer);
}

const std::string_view& MppItem::GetName() const
{
    return kItems[ID].Name;
}

std::shared_ptr<MppItemEntity> MppItem::CreateItemEntity() const
{
    return std::make_shared<MppItemEntity>(*this);
}

std::shared_ptr<MppFurnitureEntity> MppItem::CreateFurnitureEntity() const
{
    return kItems[ID].CreateFurnitureEntity();
}

MppItemRecipe MppItem::GetRecipe() const
{
    return MppItemRecipe{kItems[ID].Recipe};
}

void MppItem::Add(int count)
{
    Count += count;
}

MppItem MppItem::Remove(int count)
{
    MppAssert(Count >= count);
    Count -= count;
    MppItem other = *this;
    other.Count = count;
    return other;
}

int MppItem::GetCount() const
{
    return Count;
}

MppItemID MppItem::GetID() const
{
    return ID;
}

MppItemType MppItem::GetType() const
{
    return kItems[ID].Type;
}

int MppItem::GetColor1() const
{
    return kItems[ID].Color1;
}

int MppItem::GetColor2() const
{
    return kItems[ID].Color2;
}

int MppItem::GetColor3() const
{
    return kItems[ID].Color3;
}

int MppItem::GetColor4() const
{
    return kItems[ID].Color4;
}

int MppItem::GetColor5() const
{
    return kItems[ID].Color5;
}

int MppItem::GetPhysicsOffsetX() const
{
    return 0;
}

int MppItem::GetPhysicsOffsetY() const
{
    return 0;
}

int MppItem::GetPhysicsWidth() const
{
    return kSize;
}

int MppItem::GetPhysicsHeight() const
{
    return kSize;
}

bool MppItem::operator==(const MppItem& other) const
{
    return ID == other.ID;
}

bool MppItem::IsValid() const
{
    return ID != MppItemIDInvalid;
}
