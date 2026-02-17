#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

#include <minicraft++/log.hpp>

class MppEntity;

class MppEntityReference
{
public:
    MppEntityReference();
    MppEntityReference(const std::shared_ptr<MppEntity>& entity);
    void Visit(SavepointVisitor& visitor);
    std::shared_ptr<MppEntity> GetEntity();
    bool IsValid() const;

private:
    std::weak_ptr<MppEntity> Entity;
    SavepointID EntityID;
};

enum MppEntityCollision : uint8_t
{
    MppEntityCollisionAccepted,
    MppEntityCollisionRejected,
    MppEntityCollisionOverriden,
};

class MppEntity :
    public SavepointEntity,
    public SavepointPoly,
    public std::enable_shared_from_this<MppEntity>
{
public:
    MppEntity();
    virtual ~MppEntity() = default;
    MppEntity(const MppEntity& other) = delete;
    MppEntity& operator=(const MppEntity& other) = delete;
    MppEntity(MppEntity&& other) = delete;
    MppEntity& operator=(MppEntity&& other) = delete;
    virtual void OnCreate();
    virtual void OnAdd();
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Render() const;
    virtual void Update(uint64_t ticks) {}
    virtual bool OnAction(std::shared_ptr<MppEntity>& instigator);
    virtual bool OnInteraction(std::shared_ptr<MppEntity>& instigator);
    virtual MppEntityCollision OnCollision(std::shared_ptr<MppEntity>& instigator, int dx, int dy);
    virtual bool HasPhysics() const;
    virtual bool CanBeSaved() const;
    MppEntityReference GetReference();
    virtual void OnSetLevel(int level) {}
    bool IsColliding();
    void Unspawn();
    bool IsSpawned() const;
    void SetX(int x);
    void SetY(int y);
    int GetX() const;
    int GetY() const;
    virtual int GetSize() const = 0;
    int GetPhysicsX() const;
    int GetPhysicsY() const;
    virtual int GetPhysicsWidth() const = 0;
    virtual int GetPhysicsHeight() const = 0;
    std::pair<int, int> GetCenter() const;
    std::string GetName() const;
    int GetDistance(const std::shared_ptr<MppEntity>& entity) const;
    int IsColliding(int x, int y, int w, int h);

    template<typename T, typename... Args>
    static std::shared_ptr<T> Create(Args&&... args)
    {
        std::shared_ptr<T> entity = std::make_shared<T>(std::forward<Args>(args)...);
        if (!entity)
        {
            MppLog("Failed to create entity");
            return nullptr;
        }
        entity->OnCreate();
        return entity;
    }

    template<typename T>
    std::shared_ptr<T> Cast()
    {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

    template<typename T>
    bool IsA() const
    {
        return dynamic_cast<const T*>(this) != nullptr;
    }

protected:
    virtual int GetPhysicsOffsetX() const = 0;
    virtual int GetPhysicsOffsetY() const = 0;
    MppEntityCollision Move(int dx, int dy);
    MppEntityCollision MoveTest(int dx, int dy);

    int X;
    int Y;
    bool Spawned;
};
