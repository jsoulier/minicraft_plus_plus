#pragma once

#include <savepoint/base.hpp>
#include <savepoint/id.hpp>

#include "physics.hpp"

class MppLevel;
class MppRenderer;

class MppEntity : public SavepointBase
{
    friend class MppLevel;

public:
    MppEntity();
    virtual void Update(MppLevel& level, MppRenderer& renderer, int ticks);
    virtual void Visit(SavepointVisitor& visitor) override;
    void SetX(int x);
    void SetY(int y);
    int GetX() const;
    int GetY() const;
    int GetPhysicsX() const;
    int GetPhysicsY() const;
    virtual int GetPhysicsWidth() const = 0;
    virtual int GetPhysicsHeight() const = 0;
    virtual int GetSize() const = 0;
    virtual MppPhysicsType GetPhysicsType() const = 0;
    void Move(MppLevel& level, int dx, int dy);

protected:
    virtual int GetPhysicsOffsetX() const = 0;
    virtual int GetPhysicsOffsetY() const = 0;

private:
    void MoveAxis(MppLevel& level, int dx, int dy);
    bool MoveAxis(MppLevel& level);

protected:
    int X;
    int Y;

private:
    SavepointID ID;
};