#pragma once

class MppLevel;
class MppMobEntity;

class MppController
{
public:
    MppController(MppMobEntity& entity);
    virtual void Update(MppLevel& level, int ticks) = 0;

protected:
    MppMobEntity& Entity;
};