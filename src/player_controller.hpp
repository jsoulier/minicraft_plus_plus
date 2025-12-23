#pragma once

#include "controller.hpp"

class MppPlayerController : public MppController
{
public:
    MppPlayerController(MppMobEntity& entity);
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;

private:
    bool DrawInventory;
};