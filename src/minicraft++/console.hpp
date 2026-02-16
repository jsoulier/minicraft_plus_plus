#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include <minicraft++/input.hpp>
#include <minicraft++/menu.hpp>

class MppMobEntity;

class MppConsoleVar
{
public:
    explicit MppConsoleVar(const std::string_view& name, bool value);
    explicit MppConsoleVar(const std::string_view& name, int value);
    explicit MppConsoleVar(const std::string_view& name, float value);
    void Handle(const std::string& token);
    bool GetBool() const;
    int GetInt() const;
    float GetFloat() const;
    const std::string_view& GetName() const;

private:
    std::string_view Name;
    std::variant<bool, int, float> Value;
};

class MppConsole
    : public MppMenu
    , public MppInputHandler
{
public:
    MppConsole();
    void OnInputGainFocus() override;
    void OnInputLoseFocus() override;
    void OnInputEnter() override;
    void OnInputBackspace() override;
    void OnInputInteract() override {}
    void OnInputText(char character) override;
    void OnInputRender() const override;

    static inline MppConsoleVar CVarNavigation{"navigation", false};
    static inline MppConsoleVar CVarPhysics{"physics", false};
    static inline MppConsoleVar CVarFov{"fov", false};
    static inline MppConsoleVar CVarAction{"action", false};

private:
    void Handle();
    void HandleGive(const std::vector<std::string>& tokens);
    void HandleEntity(const std::vector<std::string>& tokens);
    void HandleTile(const std::vector<std::string>& tokens);
    void HandleKill(const std::vector<std::string>& tokens);
    void HandleKillAll(const std::vector<std::string>& tokens);
    void HandleCVar(const std::vector<std::string>& tokens);
    std::shared_ptr<MppMobEntity> GetPlayer() const;
    std::string GetEntityName(std::string name) const;

private:
    std::string Characters;
};
