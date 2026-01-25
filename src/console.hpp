#pragma once

#include <string>
#include <vector>
#include <memory>
#include <vector>

#include "input.hpp"
#include "menu.hpp"

class MppPlayerEntity;

class MppConsole
    : public MppMenu
    , public MppInputHandler
{
public:
    MppConsole();
    void OnGainFocus() override;
    void OnLoseFocus() override;
    void OnEnter() override;
    void OnBackspace() override;
    void OnInventory() override {}
    void OnTextInput(char character) override;
    void OnRender() override;

private:
    void Handle();
    void HandleGive(const std::vector<std::string>& tokens);
    void HandleSpawn(const std::vector<std::string>& tokens);
    void HandleKillAll(const std::vector<std::string>& tokens);
    std::shared_ptr<MppPlayerEntity> GetPlayer() const;

private:
    std::string Characters;
};
