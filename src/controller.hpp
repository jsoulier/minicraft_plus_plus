#pragma once

class MppLevel;
class MppMobEntity;
class MppRenderer;

class MppController
{
public:
    MppController(MppMobEntity& entity);
    virtual void Update(MppLevel& level, MppRenderer& renderer, int ticks) = 0;

protected:
    MppMobEntity& Entity;
};