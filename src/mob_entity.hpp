#pragma once

#include "entity.hpp"

class MppController;

class MppMobEntity : public MppEntity
{
public:
    MppMobEntity();
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;
    virtual void Move(MppLevel& level, int dx, int dy, int ticks);
    int GetSize() const override;

protected:
    void SetController(const std::shared_ptr<MppController>& controller);
    
    int DirectionX;
    int DirectionY;

private:
    std::shared_ptr<MppController> Controller;
};