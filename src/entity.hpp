#pragma once

#include <savepoint/base.hpp>
#include <savepoint/entity.hpp>

#include <cstdint>

class MppEntity : public SavepointEntity, public SavepointBase
{
public:
    MppEntity();
    virtual ~MppEntity() = default;
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Render();
    virtual void Update(uint64_t ticks);
    void SetX(int x);
    void SetY(int y);
    int GetX() const;
    int GetY() const;
    int GetPhysicsX() const;
    int GetPhysicsY() const;
    virtual int GetPhysicsOffsetX() const = 0;
    virtual int GetPhysicsOffsetY() const = 0;
    virtual int GetPhysicsWidth() const = 0;
    virtual int GetPhysicsHeight() const = 0;
    virtual int GetSize() const = 0;

    template<typename T>
    bool Is() const
    {
        return dynamic_cast<const T*>(this) != nullptr;
    }

protected:
    void Move(int velocityX, int velocityY);

private:
    void MoveAxis(int velocityX, int velocityY);
    bool MoveAxisTest();

protected:
    int X;
    int Y;
};
