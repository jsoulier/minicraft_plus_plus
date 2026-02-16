#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

class MppEntity;
class MppInventory;
class MppMobEntity;

class MppController 
    : public SavepointPoly
    , public std::enable_shared_from_this<MppController>
{
public:
    MppController() = default;
    ~MppController() = default;
    virtual bool Possess(const std::shared_ptr<MppMobEntity>& entity);
    virtual bool Unpossess();
    virtual void Visit(SavepointVisitor& visitor) {}
    virtual void Update(uint64_t ticks) {}
    virtual void Render() const {}
    virtual bool ActionFilter(const std::shared_ptr<MppEntity>& entity) const;
    virtual bool InteractionFilter(const std::shared_ptr<MppEntity>& entity) const;

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
    std::shared_ptr<MppInventory> GetInventory() const;

protected:
    std::weak_ptr<MppMobEntity> Entity;
};
