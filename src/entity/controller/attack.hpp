#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "../../entity.hpp"
#include "../controller.hpp"

enum MppAttackControllerState
{
    MppAttackControllerStateIdle,
    MppAttackControllerStateMove,
};

class MppAttackController : public MppController
{
public:
    MppAttackController();
    void Visit(SavepointVisitor& visitor);
    void Update(uint64_t ticks) override;

protected:
    virtual void GetAttackPosition(const std::weak_ptr<MppMobEntity>& target, int& x, int& y) const = 0;

private:
    void Idle();
    void Move();

private:
    MppEntityReference Target;
    MppAttackControllerState State;
    std::vector<std::pair<int, int>> Points;
    int PointIndex;
};
