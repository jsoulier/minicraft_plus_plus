#include <savepoint.hpp>

#include "tile.hpp"

void MppTile::Visit(SavepointVisitor& visitor)
{
    visitor(Type);
}

MppTileType MppTile::GetType() const
{
    return Type;
}

bool MppTile::operator==(const MppTile& other) const
{
    return Type == other.Type;
}

bool MppTile::IsValid() const
{
    return !operator==(kMppTileInvalid);
}
