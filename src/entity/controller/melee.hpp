#pragma once

#include <memory>

#include "attack.hpp"

class MppMeleeController : public MppAttackController
{
public:
    MppMeleeController(const std::shared_ptr<MppMobEntity>& entity);

protected:
    void GetAttackPosition(const std::weak_ptr<MppMobEntity>& target, int& x, int& y) const override;
};