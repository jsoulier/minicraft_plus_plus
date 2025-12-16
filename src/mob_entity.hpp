#pragma once

#include "entity.hpp"

class MppController;

class MppMobEntity : public MppEntity
{
public:
    MppMobEntity();
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;
    void Move(MppLevel& level, int dx, int dy, int ticks);
    int GetSize() const override;

protected:
    void SetController(const std::shared_ptr<MppController>& controller);

private:
    std::shared_ptr<MppController> Controller;
};