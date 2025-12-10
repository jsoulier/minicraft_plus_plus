#include <savepoint.hpp>

#include "entity.hpp"

MppEntity::MppEntity()
    : X{0.0f}
    , Y{0.0f}
{
}

void MppEntity::Visit(SavepointVisitor& visitor)
{
    visitor(X);
    visitor(Y);
}

void MppEntity::SetX(float x)
{
    X = x;
}

void MppEntity::SetY(float y)
{
    Y = y;
}

float MppEntity::GetX() const
{
    return X;
}

float MppEntity::GetY() const
{
    return Y;
}