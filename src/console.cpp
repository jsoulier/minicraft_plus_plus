#include <SDL3/SDL.h>

#include <algorithm>
#include <cctype>
#include <exception>
#include <sstream>
#include <string>
#include <vector>

#include "color.hpp"
#include "console.hpp"
#include "entity.hpp"
#include "entity/mob.hpp"
#include "entity/player.hpp"
#include "inventory.hpp"
#include "item.hpp"
#include "log.hpp"
#include "menu.hpp"
#include "renderer.hpp"
#include "world.hpp"

MppConsole::MppConsole()
    : Characters{}
{
    SetX1(0);
    SetY1(128);
    SetX2(256);
    SetY2(136);
}

void MppConsole::OnGainFocus() 
{
    SDL_Window** windows = SDL_GetWindows(nullptr);
    SDL_Window* window = windows[0];
    SDL_Rect rect;
    rect.x = X1;
    rect.y = Y1;
    rect.w = GetWidth();
    rect.h = GetHeight();
    SDL_SetTextInputArea(SDL_GetGrabbedWindow(), &rect, 0);
    if (!SDL_StartTextInput(window))
    {
        MppLog("Failed to start text input: %s", SDL_GetError());
    }
    SDL_free(windows);
}

void MppConsole::OnLoseFocus() 
{
    Characters.clear();
    SDL_Window** windows = SDL_GetWindows(nullptr);
    SDL_Window* window = windows[0];
    SDL_StopTextInput(window);
    SDL_free(windows);
}

void MppConsole::HandleGive(const std::vector<std::string>& tokens)
{
    std::shared_ptr<MppPlayerEntity> player = GetPlayer();
    if (!player)
    {
        return;
    }
    if (tokens.size() > 3)
    {
        MppLog("Expected \"give <item_name>\"");
        return;
    }
    if (tokens.size() < 2)
    {
        MppLog("Expected \"give <item_name>\"");
        return;
    }
    MppItem item;
    for (int i = MppItemIDInvalid + 1; i < MppItemIDCount; i++)
    {
        std::string name{MppItem{MppItemID(i)}.GetName()};
        for (int j = 0; j < name.size(); j++)
        {
            if (name[j] == ' ')
            {
                name[j] = '_';
            }
        }
        if (name == tokens[1])
        {
            item = MppItem{MppItemID(i)};
            break;
        }
    }
    if (!item.IsValid())
    {
        MppLog("Given item was invalid");
        return;
    }
    int count = 1;
    if (tokens.size() == 3)
    {
        try
        {
            count = std::stoi(tokens[2]);
        }
        catch (const std::exception& e)
        {
            MppLog("Invalid count: %s, %s", Characters.data(), e.what());
            return;
        }
    }
    for (int i = 0; i < count; i++)
    {
        player->GetInventory()->Add(item);
    }
}

void MppConsole::Handle()
{
    std::vector<std::string> tokens;
    std::istringstream stream(Characters);
    std::string token;
    while (stream >> token)
    {
        for (int i = 0; i < token.size(); i++)
        {
            token[i] = std::tolower(token[i]);
        }
        tokens.push_back(token);
    }
    if (tokens.empty())
    {
        MppLog("Found zero tokens");
        return;
    }
    if (tokens[0] == "give")
    {
        HandleGive(tokens);
    }
    else
    {
        MppLog("Unknown command: %s", tokens[0].data());
    }
}

std::shared_ptr<MppPlayerEntity> MppConsole::GetPlayer() const
{
    std::vector<std::shared_ptr<MppEntity>> entities = MppWorldGetEntities();
    for (std::shared_ptr<MppEntity>& entity : entities)
    {
        std::shared_ptr<MppPlayerEntity> player = std::dynamic_pointer_cast<MppPlayerEntity>(entity);
        if (player)
        {
            return player;
        }
    }
    MppLog("Failed to find player");
    return nullptr;
}

void MppConsole::OnEnter()
{
    Handle();
    MppInputResetInteraction();
}

void MppConsole::OnBackspace()
{
    if (!Characters.empty())
    {
        Characters.pop_back();
    }
}

void MppConsole::OnTextInput(char character) 
{
    if (character != '/')
    {
        Characters.push_back(character);
    }
}

void MppConsole::OnRender()
{
    MppMenu::Render();
    MppMenu::Render("/", kMppColorMenuForeground, X1, Y1, MppMenuAlignmentLeft);
    MppMenu::Render(Characters, kMppColorMenuForeground, X1 + 8, Y1, MppMenuAlignmentLeft);
    MppMenu::Render("^", kMppColorMenuForeground, X1 + Characters.size() * 8 + 8, Y1 + 8, MppMenuAlignmentLeft);
}
