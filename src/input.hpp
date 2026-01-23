#pragma once

#include <SDL3/SDL.h>

#include <cstdint>
#include <memory>

class MppInputHandler;

void MppInputQuit();
void MppInputSetPlayer(const std::shared_ptr<MppInputHandler>& handler);
void MppInputSetInteraction(const std::shared_ptr<MppInputHandler>& handler);
void MppInputResetPlayer();
void MppInputResetInteraction();
void MppInputUpdate(uint64_t ticks);
void MppInputRender();
void MppInputHandle(SDL_Event* event);

class MppInputHandler
{
public:
    virtual void OnGainFocus() {}
    virtual void OnLoseFocus() {}
    virtual void OnAction() {}
    virtual void OnInventory() { MppInputResetInteraction(); }
    virtual void OnUp() {}
    virtual void OnDown() {}
    virtual void OnUpArrow() {}
    virtual void OnDownArrow() {}
    virtual void OnLeft() {}
    virtual void OnRight() {}
    virtual void OnEnter() {}
    virtual void OnBackspace() {}
    virtual void OnHeldUp() {}
    virtual void OnHeldDown() {}
    virtual void OnHeldLeft() {}
    virtual void OnHeldRight() {}
    virtual void OnTextInput(char character) {}
    virtual void OnUpdate(uint64_t ticks) {}
    virtual void OnRender() {}
};
