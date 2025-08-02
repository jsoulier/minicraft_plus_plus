#pragma once

#include "entity.hpp"

class MppMobEntity : public MppEntity
{
public:
    void Update(const MppTime& time) override;
};