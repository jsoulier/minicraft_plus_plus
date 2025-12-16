#pragma once

#include <savepoint.hpp>

class MppLevel;
class MppRenderer;

class MppEntity : public SavepointBase
{
    friend class MppLevel;

public:
    MppEntity();
    virtual void Update(MppLevel& level, MppRenderer& renderer, float dt, float ticks);
    virtual void Visit(SavepointVisitor& visitor) override;
    void SetX(float x);
    void SetY(float y);
    float GetX() const;
    float GetY() const;
    float GetPhysicsX() const;
    float GetPhysicsY() const;
    virtual float GetPhysicsWidth() const = 0;
    virtual float GetPhysicsHeight() const = 0;
    virtual int GetSize() const = 0;

protected:
    virtual int GetPhysicsOffsetX() const = 0;
    virtual int GetPhysicsOffsetY() const = 0;
    void Move(MppLevel& level, float dx, float dy);

private:
    bool MoveTest(MppLevel& level);

protected:
    float X;
    float Y;

private:
    SavepointID ID;
};