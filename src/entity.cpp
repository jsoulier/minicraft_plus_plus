#include <cstdint>

#include "entity.hpp"
#include "world.hpp"

MppEntity::MppEntity()
    : X{0}
    , Y{0}
{
}

void MppEntity::Visit(SavepointVisitor& visitor)
{
    visitor(X);
    visitor(Y);
}

void MppEntity::Render()
{
}

void MppEntity::Update(uint64_t ticks)
{
}

void MppEntity::SetX(int x)
{
    X = x;
}

void MppEntity::SetY(int y)
{
    Y = y;
}

int MppEntity::GetX() const
{
    return X;
}

int MppEntity::GetY() const
{
    return Y;
}

int MppEntity::GetPhysicsX() const
{
    return X + GetPhysicsOffsetX();
}

int MppEntity::GetPhysicsY() const
{
    return Y + GetPhysicsOffsetY();
}

void MppEntity::Move(int velocityX, int velocityY)
{
    MoveAxis(velocityX, 0);
    MoveAxis(0, velocityY);
}

void MppEntity::MoveAxis(int velocityX, int velocityY)
{
    int x = X;
    int y = Y;
    X += velocityX;
    Y += velocityY;
    if (!MoveAxisTest())
    {
        X = x;
        Y = y;
    }
}

bool MppEntity::MoveAxisTest()
{
    return true;
}
