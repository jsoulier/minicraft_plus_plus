#pragma once

#include <string_view>

#include "inventory.hpp"

class MppChestInventory : public MppInventory
{
public:
    void Action() override;

protected:
    std::string_view GetName() const override;
};