#pragma once

#include "entity.hpp"

class MppItemEntity : public MppEntity
{
    SAVEPOINT_DERIVED(MppItemEntity)

public:
    void Update(const std::shared_ptr<MppLevel>& level, MppRenderer& renderer, float dt, float ticks) override;
};