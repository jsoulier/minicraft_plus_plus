#include "recipe_output_menu.hpp"

std::string_view MppRecipeOutputMenu::GetName() const
{
    return "OUTPUT";
}

int MppRecipeOutputMenu::GetX() const
{
    return 168;
}

int MppRecipeOutputMenu::GetY() const
{
    return 16;
}

int MppRecipeOutputMenu::GetWidth() const
{
    return 84;
}

int MppRecipeOutputMenu::GetHeight() const
{
    return 56;
}
