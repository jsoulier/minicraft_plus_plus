#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

class MppEntity;

class MppEntityReference
{
    friend class MppEntity;

private:
    MppEntityReference(const std::shared_ptr<MppEntity>& entity);

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
    public SavepointPoly,
    public std::enable_shared_from_this<MppEntity>
{
protected:
    MppEntity();

public:
    virtual ~MppEntity() = default;
    MppEntity(const MppEntity& other) = delete;
    MppEntity& operator=(const MppEntity& other) = delete;
    MppEntity(MppEntity&& other) = delete;
    MppEntity& operator=(MppEntity&& other) = delete;
    virtual void OnAddEntity();
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Render() const;
    virtual void Update(uint64_t ticks) {}
    virtual void OnAction(MppEntity& instigator) {}
    virtual bool OnCollision(MppEntity& instigator);
    virtual bool HasPhysics() const;
    virtual bool CanSave() const;
    MppEntityReference GetReference();
    void Kill();
    bool IsKilled() const;
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
    void GetCenter(int& x, int& y) const;
    int GetDistance(const std::shared_ptr<MppEntity>& entity) const;

protected:
    void Move(int dx, int dy);

private:
    void MoveTest(int dx, int dy);

protected:
    int X;
    int Y;

private:
    bool Killed;
};
