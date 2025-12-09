#pragma once

#include <savepoint.hpp>

class MppWorld;
class MppRenderer;

class MppEntity : public SavepointBase
{
    friend class MppWorld;

public:
    MppEntity();
    virtual void Update(MppWorld& world, MppRenderer& renderer, float dt, float ticks) = 0;
    virtual void Visit(SavepointVisitor& visitor) override;
    void SetX(float x);
    void SetY(float y);

private:
    SavepointID ID;
    float X;
    float Y;
};