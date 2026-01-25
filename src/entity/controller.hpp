#pragma once

#include <cstdint>
#include <memory>

class MppMobEntity;

class MppMobController
{
public:
    MppMobController(const std::shared_ptr<MppMobEntity>& entity);
    ~MppMobController() = default;
    virtual void Update(uint64_t ticks);

private:
    std::weak_ptr<MppMobEntity> Entity;
};