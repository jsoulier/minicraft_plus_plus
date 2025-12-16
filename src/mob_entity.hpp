#pragma once

#include "entity.hpp"

class MppController;

class MppMobEntity : public MppEntity
{
public:
    MppMobEntity();
    void Update(MppLevel& level, MppRenderer& renderer, float dt, float ticks) override;
    int GetSize() const override;

protected:
    void SetController(const std::shared_ptr<MppController>& controller);
    virtual float GetSpeed() const;

private:
    void Move(MppLevel& level, float dt, float ticks);

    std::shared_ptr<MppController> Controller;
};