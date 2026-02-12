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
    virtual void Possess(const std::shared_ptr<MppMobEntity>& entity);
    virtual void Unpossess();
    virtual void Visit(SavepointVisitor& visitor);
    virtual void Update(uint64_t ticks) {}
    virtual void RenderFromEntity() const {}
    virtual bool ActionFilter(const std::shared_ptr<MppEntity>& entity) const;
    virtual bool InteractionFilter(const std::shared_ptr<MppEntity>& entity) const;

protected:
    std::shared_ptr<MppInventory> GetInventory() const;

protected:
    std::weak_ptr<MppMobEntity> Entity;
};
