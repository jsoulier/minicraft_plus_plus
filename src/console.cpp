#include <SDL3/SDL.h>
#include <savepoint/savepoint.hpp>

#include <algorithm>
#include <cctype>
#include <exception>
#include <format>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "assert.hpp"
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
#include "tile.hpp"
#include "world.hpp"

static std::vector<MppConsoleVar*>& GetVariables()
{
    // SIOF
    static std::vector<MppConsoleVar*> variables;
    return variables;
}

MppConsoleVar::MppConsoleVar(const std::string_view& name, bool value)
    : Name{name}
    , Value{value}
{
    GetVariables().push_back(this);
}

MppConsoleVar::MppConsoleVar(const std::string_view& name, int value)
    : Name{name}
    , Value{value}
{
    GetVariables().push_back(this);
}

MppConsoleVar::MppConsoleVar(const std::string_view& name, float value)
    : Name{name}
    , Value{value}
{
    GetVariables().push_back(this);
}

void MppConsoleVar::Handle(const std::string& token)
{
    try
    {
        if (std::get_if<bool>(&Value))
        {
            int value = 0;
            if (token == "true")
            {
                value = 1;
            }
            else if (token == "false")
            {
                value = 0;
            }
            else
            {
                value = std::stoi(token);
                if (value != 0 && value != 1)
                {
                    MppLog("Failed to handle bool cvar: %s, %s", Name.data(), token.data());
                    return;
                }
            }
            Value = bool(value);
        }
        else if (std::get_if<int>(&Value))
        {
            Value = int(std::stoi(token));
        }
        else if (std::get_if<float>(&Value))
        {
            Value = float(std::stof(token));
        }
        else
        {
            MppAssert(false);
        }
    }
    catch (const std::exception& e)
    {
        MppLog("Failed to handle cvar: %s, %s", Name.data(), token.data());
    }
}

bool MppConsoleVar::GetBool() const
{
    MppAssert(std::get_if<bool>(&Value));
    return std::get<bool>(Value);
}

int MppConsoleVar::GetInt() const
{
    MppAssert(std::get_if<int>(&Value));
    return std::get<int>(Value);
}

float MppConsoleVar::GetFloat() const
{
    MppAssert(std::get_if<float>(&Value));
    return std::get<float>(Value);
}

const std::string_view& MppConsoleVar::GetName() const
{
    return Name;
}

MppConsole::MppConsole()
    : Characters{}
{
    SetX1(0);
    SetY1(0);
    SetX2(256);
    SetY2(8);
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
    if (tokens.size() < 2 || tokens.size() > 3)
    {
        MppLog("Expected give <item_name>");
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

void MppConsole::HandleEntity(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 4)
    {
        MppLog("Expected entity <entity_name> <x> <y>");
        return;
    }
    std::string name = GetEntityName(tokens[1]);
    SavepointDerivedFunction function = SavepointGetDerivedFunction(name);
    if (!function)
    {
        MppLog("Unknown entity: %s", tokens[1].data());
        return;
    }
    try
    {
        std::shared_ptr<MppEntity> entity{dynamic_cast<MppEntity*>(function())};
        entity->SetX(std::stoi(tokens[2]));
        entity->SetY(std::stoi(tokens[3]));
        MppWorldAddEntity(entity);
    }
    catch (const std::exception& e)
    {
        MppLog("Invalid position: %s, %s", tokens[2].data(), tokens[3].data());
    }
}

void MppConsole::HandleTile(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 4)
    {
        MppLog("Expected tile <tile_name> <x> <y>");
        return;
    }
    MppTile tile;
    for (int i = MppTileIDInvalid + 1; i < MppTileIDCount; i++)
    {
        std::string name{MppTile{MppTileID(i)}.GetName()};
        for (int j = 0; j < name.size(); j++)
        {
            if (name[j] == ' ')
            {
                name[j] = '_';
            }
        }
        if (name == tokens[1])
        {
            tile = MppTile{MppTileID(i)};
            break;
        }
    }
    if (!tile.IsValid())
    {
        MppLog("Unknown tile: %s", tokens[1].data());
        return;
    }
    try
    {
        int x = std::stoi(tokens[2]);
        int y = std::stoi(tokens[3]);
        MppWorldSetTile(tile, x, y);
    }
    catch (const std::exception& e)
    {
        MppLog("Invalid position: %s, %s", tokens[2].data(), tokens[3].data());
    }
}

void MppConsole::HandleKill(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 2)
    {
        MppLog("Expected kill <entity_name>");
        return;
    }
    std::string name = GetEntityName(tokens[1]);
    for (std::shared_ptr<MppEntity>& entity : MppWorldGetEntities())
    {
        if (entity->GetName() == name)
        {
            entity->Kill();
        }
    }
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

void MppConsole::HandleCVar(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 3)
    {
        MppLog("Expected cvar <cvar_name> <value>");
        return;
    }
    for (MppConsoleVar* variable : GetVariables())
    {
        if (variable->GetName() == tokens[1])
        {
            variable->Handle(tokens[2]);
            return;
        }
    }
    MppLog("Unknown cvar: %s", tokens[1].data());
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
    else if (tokens[0] == "entity")
    {
        HandleEntity(tokens);
    }
    else if (tokens[0] == "tile")
    {
        HandleTile(tokens);
    }
    else if (tokens[0] == "kill")
    {
        HandleKill(tokens);
    }
    else if (tokens[0] == "killall")
    {
        HandleKillAll(tokens);
    }
    else if (tokens[0] == "cvar")
    {
        HandleCVar(tokens);
    }
    else
    {
        MppLog("Unknown command: %s", tokens[0].data());
    }
}

std::shared_ptr<MppPlayerEntity> MppConsole::GetPlayer() const
{
    for (std::shared_ptr<MppEntity>& entity : MppWorldGetEntities())
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

std::string MppConsole::GetEntityName(std::string name) const
{
    // TODO: doesn't handle underscore separators
    MppAssert(name.find('_') == std::string::npos);
    name[0] = std::toupper(name[0]);
    return std::format("Mpp{}Entity", name);
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
    MppMenu::Render("/", kMppColorMenuUnlockedForeground, X1, Y1, MppMenuAlignmentLeft);
    MppMenu::Render(Characters, kMppColorMenuUnlockedForeground, X1 + 8, Y1, MppMenuAlignmentLeft);
    MppMenu::Render("^", kMppColorMenuUnlockedForeground, X1 + Characters.size() * 8 + 8, Y1 + 8, MppMenuAlignmentLeft);
}
