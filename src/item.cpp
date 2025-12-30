#include <SDL3/SDL.h>
#include <savepoint/visitor.hpp>

#include <string_view>

#include "color.hpp"
#include "item.hpp"

struct
{
    std::string_view Name;
    MppItemFlag Flag;
    MppItemType Type;
    int Color1;
    int Color2;
    int Color3;
    int Color4;
    int Color5;
    int SpriteX;
    int SpriteY;
}
static constexpr kItems[MppItemIDCount] =
{
    /* wood */
    {
        .Name = "WOOD",
        .Flag = MppItemFlagMaterial,
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
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 0,
        .SpriteY = 13,
    },
    /* iron chestplate */
    {
        .Name = "IRON CHESTPLATE",
        .Flag = MppItemFlagArmor,
        .Type = MppItemTypeChestplate,
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
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 2,
        .SpriteY = 13,
    },
    /* iron chestplate */
    {
        .Name = "IRON BOOTS",
        .Flag = MppItemFlagArmor,
        .Type = MppItemTypeBoots,
        .Color1 = kMppColorIron1,
        .Color2 = kMppColorIron2,
        .Color3 = kMppColorIron3,
        .Color4 = kMppColorIron4,
        .Color5 = kMppColorIron5,
        .SpriteX = 3,
        .SpriteY = 13,
    },
};

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
