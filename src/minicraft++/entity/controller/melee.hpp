#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include <minicraft++/entity/entity.hpp>
#include <minicraft++/entity/controller/npc.hpp>

enum MppMeleeControllerState
{
    MppMeleeControllerStateIdle,
    MppMeleeControllerStateMove,
};

class MppMeleeController : public MppNPCController
{
    SAVEPOINT_POLY(MppMeleeController)

public:
    MppMeleeController();
    void Visit(SavepointVisitor& visitor);
    void Update(uint64_t ticks) override;

protected:
    int GetMaxNavigateRandomDistance() const override;

private:
    void Idle();
    void Move();

private:
    MppEntityReference Target;
    MppMeleeControllerState State;
};
