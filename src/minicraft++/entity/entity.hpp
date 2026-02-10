#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>
#include <string>

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
    std::string GetName() const;
    virtual void OnAddEntity();
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Render() const;
    virtual void Update(uint64_t ticks) {}
    virtual bool OnAction(MppEntity& instigator);
    virtual bool OnCollision(MppEntity& instigator, int dx, int dy);
    virtual bool HasPhysics() const;
    virtual bool CanSave() const;
    MppEntityReference GetReference();
    bool IsColliding();
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
    std::pair<int, int> GetCenter() const;
    int GetDistance(const std::shared_ptr<MppEntity>& entity) const;

    template<typename T>
    bool IsA() const
    {
        return dynamic_cast<const T*>(this) != nullptr;
    }

protected:
    bool Move(int dx, int dy);

private:
    bool MoveTest(int dx, int dy);

protected:
    int X;
    int Y;

private:
    bool Killed;
};
