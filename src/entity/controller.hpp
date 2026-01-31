#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

class MppInventory;
class MppMobEntity;

class MppController 
    : public SavepointBase
    , public std::enable_shared_from_this<MppController>
{
public:
    MppController() = default;
    MppController(const std::shared_ptr<MppMobEntity>& entity);
    ~MppController() = default;
    void SetEntity(const std::shared_ptr<MppMobEntity>& entity);
    virtual void OnAddEntity() {}
    virtual void Visit(SavepointVisitor& visitor);
    virtual void Update(uint64_t ticks) {}

protected:
    void PickRandomTarget(int maxDistance, int& x, int& y);
    std::shared_ptr<MppInventory> GetInventory();

protected:
    std::weak_ptr<MppMobEntity> Entity;
};
