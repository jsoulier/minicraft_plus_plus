#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include "attack.hpp"

class MppMeleeController : public MppAttackController
{
    SAVEPOINT_DERIVED(MppMeleeController)

public:
    MppMeleeController() = default;

protected:
    void GetAttackPosition(const std::weak_ptr<MppMobEntity>& target, int& x, int& y) const override;
};