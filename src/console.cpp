#include <SDL3/SDL.h>

#include <algorithm>
#include <cctype>
#include <exception>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "color.hpp"
#include "console.hpp"
#include "entity.hpp"
#include "entity/chest.hpp"
#include "entity/furnace.hpp"
#include "entity/item.hpp"
#include "entity/player.hpp"
#include "entity/workbench.hpp"
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
        MppLog("Unknown item: %s", tokens[1].data());
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

void MppConsole::HandleSpawn(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 4)
    {
        MppLog("Expected \"spawn <entity_name> <x> <y>\"");
        return;
    }
    std::shared_ptr<MppEntity> entity;
    if (tokens[1] == "chest")
    {
        entity = std::make_shared<MppChestEntity>();
    }
    else if (tokens[1] == "furnace")
    {
        entity = std::make_shared<MppFurnaceEntity>();
    }
    else if (tokens[1] == "item")
    {
        entity = std::make_shared<MppItemEntity>();
    }
    else if (tokens[1] == "player")
    {
        if (!GetPlayer())
        {
            entity = std::make_shared<MppPlayerEntity>();
        }
        else
        {
            MppLog("Tried to add another player");
            return;
        }
    }
    else if (tokens[1] == "workbench")
    {
        entity = std::make_shared<MppWorkbenchEntity>();
    }
    else
    {
        MppLog("Unknown entity: %s", tokens[1].data());
        return;
    }
    try
    {
        entity->SetX(std::stoi(tokens[2]));
        entity->SetY(std::stoi(tokens[3]));
    }
    catch (const std::exception& e)
    {
        MppLog("Invalid position: %s, %s", tokens[2].data(), tokens[3].data());
        return;
    }
    MppWorldAddEntity(entity);
}

void MppConsole::HandleKillAll(const std::vector<std::string>& tokens)
{
    std::vector<std::shared_ptr<MppEntity>> entities = MppWorldGetEntities();
    for (std::shared_ptr<MppEntity>& entity : entities)
    {
        // TODO: should enable in the future
        std::shared_ptr<MppPlayerEntity> player = std::dynamic_pointer_cast<MppPlayerEntity>(entity);
        if (!player)
        {
            entity->Kill();
        }
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
    else if (tokens[0] == "spawn")
    {
        HandleSpawn(tokens);
    }
    else if (tokens[0] == "killall")
    {
        HandleKillAll(tokens);
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
