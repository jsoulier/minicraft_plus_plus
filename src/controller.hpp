#pragma once

class MppMobEntity;

class MppController
{
public:
    MppController(MppMobEntity& entity);

protected:
    MppMobEntity& Entity;
};