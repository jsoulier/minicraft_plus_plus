#include <savepoint.hpp>

#include "item.hpp"

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
}

int MppItem::GetColor1() const
{
    return 0;
}

int MppItem::GetColor2() const
{
    return 0;
}

int MppItem::GetColor3() const
{
    return 0;
}

int MppItem::GetColor4() const
{
    return 0;
}

int MppItem::GetColor5() const
{
    return 0;
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
    return 8;
}

int MppItem::GetPhysicsHeight() const
{
    return 8;
}

bool MppItem::operator==(const MppItem& other) const
{
    return ID == other.ID;
}

bool MppItem::IsValid() const
{
    return !operator==(kMppItemInvalid);
}
