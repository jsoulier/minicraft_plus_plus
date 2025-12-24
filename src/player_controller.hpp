#pragma once

#include <memory>

#include "controller.hpp"

class MppMenuList;

class MppPlayerController : public MppController
{
public:
    MppPlayerController(MppMobEntity& entity);
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;

private:
    void Toggle(const std::shared_ptr<MppMenuList>& menuList);

    std::shared_ptr<MppMenuList> MenuList;
};