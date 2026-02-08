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
    void SetEntity(const std::shared_ptr<MppMobEntity>& entity);
    virtual void OnAddEntity() {}
    virtual void Visit(SavepointVisitor& visitor);
    virtual void Update(uint64_t ticks) {}
    virtual bool ActionFilter(const std::shared_ptr<MppEntity>& entity) const;

protected:
    std::shared_ptr<MppInventory> GetInventory();

protected:
    std::weak_ptr<MppMobEntity> Entity;
};
