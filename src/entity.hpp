#pragma once

#include <savepoint.hpp>

class MppLevel;
class MppRenderer;

class MppEntity : public SavepointBase
{
    friend class MppLevel;

public:
    MppEntity();
    virtual void Update(MppLevel& level, MppRenderer& renderer, float dt, float ticks) = 0;
    virtual void Visit(SavepointVisitor& visitor) override;
    void SetX(float x);
    void SetY(float y);
    float GetX() const;
    float GetY() const;

protected:
    float X;
    float Y;

private:
    SavepointID ID;
};