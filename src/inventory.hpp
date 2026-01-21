#pragma once

#include <savepoint/fwd.hpp>

#include <vector>

#include "input.hpp"
#include "item.hpp"

class MppInventory
{
public:
    void Visit(SavepointVisitor& visitor);
    void OnAction();
    void OnUp();
    void OnDown();
    void OnHeldUp();
    void OnHeldDown();

private:
    std::vector<MppItem> Items;
};