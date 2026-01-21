#pragma once

#include <savepoint/base.hpp>
#include <savepoint/entity.hpp>

#include <cstdint>
#include <memory>
#include <utility>

class MppEntity :
    public SavepointEntity,
    public SavepointBase,
    public std::enable_shared_from_this<MppEntity>
{
protected:
    MppEntity();

public:
    virtual ~MppEntity() = default;
    virtual void OnAddEntity() {}
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Render() {}
    virtual void Update(uint64_t ticks) {}
    virtual void OnAction(MppEntity& instigator) {}
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

    template<typename T>
    T& Cast()
    {
        return *dynamic_cast<T*>(this);
    }

    template<typename T>
    std::shared_ptr<T> SharedFromThis()
    {
        return std::dynamic_pointer_cast<T>(shared_from_this());
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
