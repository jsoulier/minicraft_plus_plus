#pragma once

#include <savepoint/fwd.hpp>

#include <vector>

#include "input.hpp"
#include "item.hpp"
#include "menu.hpp"

class MppInventory
    : public MppMenu
    , public MppInputHandler
{
public:
    void Visit(SavepointVisitor& visitor);
    void Render() override;
    void OnAction();
    void OnUp();
    void OnDown();
    void OnHeldUp();
    void OnHeldDown();
    void OnRender() override;

private:
    std::vector<MppItem> Items;
};
