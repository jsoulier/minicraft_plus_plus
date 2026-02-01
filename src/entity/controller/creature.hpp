#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>

#include "npc.hpp"

class MppCreatureEntity;

enum MppCreatureControllerState
{
    MppCreatureControllerStateIdle,
    MppCreatureControllerStateMove,
};
    
class MppCreatureController : public MppNPCController
{
    SAVEPOINT_DERIVED(MppCreatureController)

public:
    MppCreatureController();
    void Visit(SavepointVisitor& visitor) override;
    void Update(uint64_t ticks) override;
    
protected:
    int GetMaxNavigateRandomDistance() const override;

private:
    std::shared_ptr<MppCreatureEntity> GetCreature();
    void Idle();
    void Move();

private:
    MppCreatureControllerState State;
    int IdleTicks;
};
