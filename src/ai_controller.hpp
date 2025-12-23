#pragma once

#include <memory>

#include "controller.hpp"

class MppAIController : public MppController
{
public:
    MppAIController(MppMobEntity& entity);
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;
};