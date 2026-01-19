#pragma once

#include <savepoint/base.hpp>

#include <memory>

#include "humanoid.hpp"

class MppInputHandler;
class Handler;

class MppPlayerEntity final : public MppHumanoidEntity
{
    SAVEPOINT_DERIVED(MppPlayerEntity)

    friend class Handler;

public:
    MppPlayerEntity();
    void Update(uint64_t ticks) override;
    void Render() override;

private:
    std::shared_ptr<MppInputHandler> InputHandler;
};