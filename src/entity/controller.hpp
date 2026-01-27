#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

class MppMobEntity;

class MppMobController
{
public:
    MppMobController(const std::shared_ptr<MppMobEntity>& entity);
    ~MppMobController() = default;
    void SetEntity(const std::shared_ptr<MppMobEntity>& entity);
    virtual void Visit(SavepointVisitor& visitor);
    virtual void Update(uint64_t ticks) = 0;

protected:
    std::weak_ptr<MppMobEntity> Entity;
};