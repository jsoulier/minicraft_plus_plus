#include <savepoint.hpp>

#include <string_view>

#include "item.hpp"

struct
{
    std::string_view Name;
    MppItemType Type;
    MppItemToolType ToolType;
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
        .Type = MppItemTypeMaterial,
        .ToolType = MppItemToolTypeNone,
        .Color1 = 0,
        .Color2 = 0,
        .Color3 = 0,
        .Color4 = 0,
        .Color5 = 0,
        .SpriteX = 0,
        .SpriteY = 0,
    },
    /* iron armor */
    {
        .Name = "IRON ARMOR",
        .Type = MppItemTypeArmor,
        .ToolType = MppItemToolTypeNone,
        .Color1 = 0,
        .Color2 = 0,
        .Color3 = 0,
        .Color4 = 0,
        .Color5 = 0,
        .SpriteX = 0,
        .SpriteY = 0,
    },
};

MppItem::MppItem(MppItemID id)
    : ID{id}
{
}

void MppItem::Visit(SavepointVisitor& visitor)
{
    if (visitor.IsWriter())
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

MppItemType MppItem::GetType() const
{
    return kItems[ID].Type;
}

MppItemToolType MppItem::GetToolType() const
{
    return kItems[ID].ToolType;
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
