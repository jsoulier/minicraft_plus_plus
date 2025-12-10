#pragma once

#include <memory>

#include "entity.hpp"

class MppController;

class MppMobEntity : public MppEntity
{
public:
    MppMobEntity();
    void Update(const std::shared_ptr<MppLevel>& level, MppRenderer& renderer, float dt, float ticks) override;

protected:
    void SetController(const std::shared_ptr<MppController>& controller);

private:
    std::shared_ptr<MppController> Controller;
};