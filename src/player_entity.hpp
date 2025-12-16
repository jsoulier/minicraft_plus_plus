#pragma once

#include "humanoid_entity.hpp"

class MppPlayerEntity : public MppHumanoidEntity
{
    SAVEPOINT_DERIVED(MppPlayerEntity)

public:
    MppPlayerEntity();
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;
};