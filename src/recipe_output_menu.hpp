#pragma once

#include "menu.hpp"

class MppRecipeOutputMenu : public MppMenu
{
public:

protected:
    std::string_view GetName() const override;
    int GetX() const override;
    int GetY() const override;
    int GetWidth() const override;
    int GetHeight() const override;
};