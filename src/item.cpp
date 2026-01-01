#include <SDL3/SDL.h>
#include <savepoint/visitor.hpp>

#include <memory>
#include <string_view>
#include <unordered_map>

#include "chest_entity.hpp"
#include "color.hpp"
#include "furnace_entity.hpp"
#include "inventory.hpp"
#include "item.hpp"
#include "item_entity.hpp"
#include "workbench_entity.hpp"

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

struct
{
    std::string_view Name;
    MppItemFlag Flag;
    MppItemType Type;
    MppItemRecipe Recipe;
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
    /* wood */
    {
        .Name = "WOOD",
        .Flag = MppItemFlagNone,
        .Type = MppItemTypeNone,
        .Color1 = kMppColorWood1,
        .Color2 = kMppColorWood2,
        .Color3 = kMppColorWood3,
        .Color4 = 0,
        .Color5 = 0,
        .SpriteX = 0,
        .SpriteY = 12,
    },
    /* apple */
    {
        .Name = "APPLE",
        .Flag = MppItemFlagConsumable,
        .Type = MppItemTypeNone,
        .Color1 = 500,
        .Color2 = 700,
        .Color3 = 900,
        .Color4 = 740,
        .Color5 = 90,
        .SpriteX = 1,
        .SpriteY = 12,
    },
    /* iron helmet */
    {
        .Name = "IRON HELMET",
        .Flag = MppItemFlagArmor,
        .Type = MppItemTypeHelmet,
        .Recipe = MppItemRecipe(
            MppItemRecipeTypeWorkbench,
            MppItemIDIronHelmet,
            {{MppItemIDIronBar, 5}}),
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 0,
        .SpriteY = 13,
    },
    /* iron cuirass */
    {
        .Name = "IRON CUIRASS",
        .Flag = MppItemFlagArmor,
        .Type = MppItemTypeCuirass,
        .Recipe = MppItemRecipe(
            MppItemRecipeTypeWorkbench,
            MppItemIDIronCuirass,
            {{MppItemIDIronBar, 8}}),
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 1,
        .SpriteY = 13,
    },
    /* iron leggings */
    {
        .Name = "IRON LEGGINGS",
        .Flag = MppItemFlagArmor,
        .Type = MppItemTypeLeggings,
        .Recipe = MppItemRecipe(
            MppItemRecipeTypeWorkbench,
            MppItemIDIronLeggings,
            {{MppItemIDIronBar, 7}}),
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 2,
        .SpriteY = 13,
    },
    /* iron boots */
    {
        .Name = "IRON BOOTS",
        .Flag = MppItemFlagArmor,
        .Type = MppItemTypeBoots,
        .Recipe = MppItemRecipe(
            MppItemRecipeTypeWorkbench,
            MppItemIDIronBoots,
            {{MppItemIDIronBar, 4}}),
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 3,
        .SpriteY = 13,
    },
    /* iron ore */
    {
        .Name = "IRON ORE",
        .Flag = MppItemFlagNone,
        .Type = MppItemTypeNone,
        .Color1 = 0,
        .Color2 = 0,
        .Color3 = 0,
        .Color4 = 0,
        .Color5 = 0,
        .SpriteX = 0,
        .SpriteY = 0,
    },
    /* iron bar */
    {
        .Name = "IRON BAR",
        .Flag = MppItemFlagNone,
        .Type = MppItemTypeNone,
        .Recipe = MppItemRecipe(
            MppItemRecipeTypeFurnace,
            MppItemIDIronBar,
            {{MppItemIDIronOre, 1}}),
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 0,
        .SpriteY = 12,
    },
    /* stone */
    {
        .Name = "STONE",
        .Flag = MppItemFlagNone,
        .Type = MppItemTypeNone,
        .Color1 = kMppColorStone1,
        .Color2 = kMppColorStone2,
        .Color3 = kMppColorStone3,
        .Color4 = kMppColorStone4,
        .Color5 = 0,
        .SpriteX = 2,
        .SpriteY = 12,
    },
    /* workbench */
    {
        .Name = "WORKBENCH",
        .Flag = MppItemFlagHeld,
        .Type = MppItemTypeFurniture,
        .Recipe = MppItemRecipe(
            MppItemRecipeTypeHand,
            MppItemIDWorkbench,
            {{MppItemIDWood, 4}}),
        .CreateFurnitureEntity = CreateWorkbenchEntity,
        .Color1 = kMppColorWorkbench1,
        .Color2 = kMppColorWorkbench2,
        .Color3 = kMppColorWorkbench3,
        .Color4 = kMppColorWorkbench4,
        .Color5 = kMppColorWorkbench5,
        .SpriteX = 0,
        .SpriteY = 14,
    },
    /* furnace */
    {
        .Name = "FURNACE",
        .Flag = MppItemFlagHeld,
        .Type = MppItemTypeFurniture,
        .Recipe = MppItemRecipe(
            MppItemRecipeTypeWorkbench,
            MppItemIDFurnace,
            {{MppItemIDStone, 8}}),
        .CreateFurnitureEntity = CreateFurnaceEntity,
        .Color1 = kMppColorFurnace1,
        .Color2 = kMppColorFurnace2,
        .Color3 = kMppColorFurnace3,
        .Color4 = kMppColorFurnace4,
        .Color5 = kMppColorFurnace5,
        .SpriteX = 1,
        .SpriteY = 14,
    },
    /* chest */
    {
        .Name = "CHEST",
        .Flag = MppItemFlagHeld,
        .Type = MppItemTypeFurniture,
        .Recipe = MppItemRecipe(
            MppItemRecipeTypeWorkbench,
            MppItemIDWorkbench,
            {{MppItemIDWood, 8}}),
        .CreateFurnitureEntity = CreateChestEntity,
        .Color1 = kMppColorChest1,
        .Color2 = kMppColorChest2,
        .Color3 = kMppColorChest3,
        .Color4 = kMppColorChest4,
        .Color5 = kMppColorChest5,
        .SpriteX = 2,
        .SpriteY = 14,
    },
};

MppItemRecipe::MppItemRecipe()
    : Type{}
    , Item{MppItemIDInvalid}
    , Materials{}
{
}

MppItemRecipe::MppItemRecipe(MppItemRecipeType type, MppItemID item, const std::unordered_map<MppItemID, int>& materials)
    : Type{type}
    , Item{item}
    , Materials{materials}
{
}

void MppItemRecipe::Craft(std::shared_ptr<MppInventory>& inventory)
{
    for (const auto& [id, count] : Materials)
    {
        if (!inventory->Remove(id))
        {
            SDL_Log("Tried to craft with missing materials: %d", Item);
            return;
        }
    }
    inventory->Add(Item);
}

bool MppItemRecipe::IsValid() const
{
    return Item != MppItemIDInvalid;
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
        SDL_assert(IsValid());
    }
    visitor(ID);
    visitor(Count);
}

void MppItem::AddItem()
{
    Count++;
}

void MppItem::RemoveItem()
{
    Count--;
}

int MppItem::GetItems() const
{
    return Count;
}

std::string_view MppItem::GetName() const
{
    return kItems[ID].Name;
}

MppItemID MppItem::GetID() const
{
    return ID;
}

MppItemFlag MppItem::GetFlag() const
{
    return kItems[ID].Flag;
}

MppItemType MppItem::GetType() const
{
    return kItems[ID].Type;
}

std::shared_ptr<MppItemEntity> MppItem::CreateItemEntity() const
{
    return std::make_shared<MppItemEntity>(*this);
}

std::shared_ptr<MppFurnitureEntity> MppItem::CreateFurnitureEntity() const
{
    return kItems[ID].CreateFurnitureEntity();
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

int MppItem::GetSpriteX() const
{
    return kItems[ID].SpriteX;
}

int MppItem::GetSpriteY() const
{
    return kItems[ID].SpriteY;
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
    return kWidth;
}

int MppItem::GetPhysicsHeight() const
{
    return kWidth;
}

bool MppItem::operator==(const MppItem& other) const
{
    return ID == other.ID;
}

bool MppItem::IsValid() const
{
    return !operator==(kMppItemInvalid);
}
