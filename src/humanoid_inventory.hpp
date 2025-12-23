#pragma once

#include <savepoint_fwd.hpp>

#include "inventory.hpp"

class MppHumanoidInventory : public MppInventory
{
public:
    MppHumanoidInventory();
    void Visit(SavepointVisitor& visitor);
};