#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include "controller.hpp"

enum MppHostileMobControllerState
{
    MppHostileMobControllerStateIdle,
    MppHostileMobControllerStateMove,
    MppHostileMobControllerStateAttackCooldown,
};

class MppHostileMobController : public MppMobController
{
public:
    MppHostileMobController(const std::shared_ptr<MppMobEntity>& entity);
    void Visit(SavepointVisitor& visitor);
    void Update(uint64_t ticks) override;
};