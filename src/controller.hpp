#pragma once

class MppMobEntity;

class MppController
{
public:
    MppController(MppMobEntity& entity);
    virtual void GetMovement(float& dx, float& dy) = 0;

protected:
    MppMobEntity& Entity;
};