#pragma once

#include <memory>

#include "controller.hpp"

class MppAIController : public MppController
{
public:
    MppAIController(MppMobEntity& entity);
    void GetMovement(float& dx, float& dy) override;
};