#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>
#include <string>

class MppEntity;

class MppEntityReference
{
    friend class MppEntity;

private:
    MppEntityReference(std::shared_ptr<MppEntity>& entity);

public:
    MppEntityReference();
    void Visit(SavepointVisitor& visitor);
    void Update();
    std::shared_ptr<MppEntity> GetEntity() const;
    bool IsValid() const;

private:
    std::weak_ptr<MppEntity> Entity;
    SavepointID EntityID;
};

class MppEntity :
    public SavepointEntity,
    public SavepointBase,
    public std::enable_shared_from_this<MppEntity>
{
protected:
    MppEntity();

public:
    virtual ~MppEntity() = default;
    virtual void OnAddEntity();
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Render() const;
    virtual void Update(uint64_t ticks) {}
    virtual void OnAction(MppEntity& instigator) {}
    virtual void OnCollision(MppEntity& instigator) {}
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
    void Kill();
    bool IsDead() const;
    std::string GetName() const;
    int GetDistance(const std::shared_ptr<MppEntity>& entity) const;
    MppEntityReference GetReference();

protected:
    void Move(int dx, int dy);

private:
    void MoveAxis(int dx, int dy);
    bool MoveAxisTest();

protected:
    int X;
    int Y;

private:
    bool Dead;
};
