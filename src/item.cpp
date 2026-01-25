#include <SDL3/SDL.h>
#include <savepoint/savepoint.hpp>

#include <memory>
#include <string_view>

#include "assert.hpp"
#include "color.hpp"
#include "entity/chest.hpp"
#include "entity/furnace.hpp"
#include "entity/item.hpp"
#include "entity/workbench.hpp"
#include "item.hpp"
#include "renderer.hpp"

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
    MppItemType Type;
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
        .Color1 = kMppColorWood1,
        .Color2 = kMppColorWood2,
        .Color3 = kMppColorWood3,
        .Color4 = 0,
        .Color5 = 0,
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
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 0,
        .SpriteY = 13,
    },
    {
        .Name = "iron cuirass",
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 1,
        .SpriteY = 13,
    },
    {
        .Name = "iron leggings",
        .Type = MppItemTypeLeggings,
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 2,
        .SpriteY = 13,
    },
    {
        .Name = "iron boots",
        .Type = MppItemTypeBoots,
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 3,
        .SpriteY = 13,
    },
    {
        .Name = "iron ore",
        .Type = MppItemTypeConsumable,
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 2,
        .SpriteY = 12,
    },
    {
        .Name = "iron bar",
        .Type = MppItemTypeConsumable,
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 0,
        .SpriteY = 12,
    },
    {
        .Name = "stone",
        .Type = MppItemTypeConsumable,
        .Color1 = kMppColorStone1,
        .Color2 = kMppColorStone2,
        .Color3 = kMppColorStone3,
        .Color4 = kMppColorStone4,
        .Color5 = 0,
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
        .Type = MppItemTypeEffect,
        .Color1 = 0,
        .Color2 = 0,
        .Color3 = 0,
        .Color4 = 0,
        .Color5 = 0,
        .SpriteX = 0,
        .SpriteY = 16,
    },
    {
        .Name = "energy",
        .Type = MppItemTypeEffect,
        .Color1 = 0,
        .Color2 = 0,
        .Color3 = 0,
        .Color4 = 0,
        .Color5 = 0,
        .SpriteX = 1,
        .SpriteY = 16,
    },
    {
        .Name = "food",
        .Type = MppItemTypeEffect,
        .Color1 = 0,
        .Color2 = 0,
        .Color3 = 0,
        .Color4 = 0,
        .Color5 = 0,
        .SpriteX = 2,
        .SpriteY = 16,
    },
};

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
        SDL_assert(IsValid());
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

std::string_view MppItem::GetName() const
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

void MppItem::Add()
{
    MppAssert(Count > 0);
    Count++;
}

MppItem MppItem::Remove()
{
    MppAssert(Count > 0);
    Count--;
    MppItem other = *this;
    other.Count = 1;
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
