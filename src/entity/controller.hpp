#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

class MppMobEntity;

class MppController
{
public:
    MppController(const std::shared_ptr<MppMobEntity>& entity);
    ~MppController() = default;
    void SetEntity(const std::shared_ptr<MppMobEntity>& entity);
    virtual void Visit(SavepointVisitor& visitor);
    virtual void Update(uint64_t ticks) = 0;

protected:
    void PickRandomTarget(int maxDistance, int& x, int& y);

protected:
    std::weak_ptr<MppMobEntity> Entity;
};