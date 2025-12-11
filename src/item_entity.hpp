#pragma once

#include "entity.hpp"

class MppItemEntity : public MppEntity
{
    SAVEPOINT_DERIVED(MppItemEntity)

public:
    void Update(MppLevel& level, MppRenderer& renderer, float dt, float ticks) override;
};