#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>

#include "../controller.hpp"

class MppCreatureEntity;

enum MppCreatureControllerState
{
    MppCreatureControllerStateIdle,
    MppCreatureControllerStateMove,
};
    
class MppCreatureController : public MppController
{
public:
    MppCreatureController(const std::shared_ptr<MppMobEntity>& entity);
    void Visit(SavepointVisitor& visitor) override;
    void Update(uint64_t ticks) override;

private:
    std::shared_ptr<MppCreatureEntity> GetCreature();
    void Idle();
    void Move();

private:
    MppCreatureControllerState State;
    int TargetX;
    int TargetY;
    int IdleTicks;
};
