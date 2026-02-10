#pragma once

#include <SDL3/SDL.h>

#include <cstdint>
#include <memory>

class MppInputHandler
{
public:
    virtual void OnGainFocus() {}
    virtual void OnLoseFocus() {}
    virtual void OnUpdate(uint64_t ticks) {}
    virtual void OnRender() {}
    virtual void OnHeldUp() {}
    virtual void OnHeldDown() {}
    virtual void OnHeldLeft() {}
    virtual void OnHeldRight() {}
    virtual void OnAction() {}
    virtual void OnDrop() {}
    virtual void OnInteract();
    virtual void OnExit();
    virtual void OnUp() {}
    virtual void OnDown() {}
    virtual void OnLeft() {}
    virtual void OnRight() {}
    virtual void OnUpArrow() {}
    virtual void OnDownArrow() {}
    virtual void OnLeftArrow() {}
    virtual void OnRightArrow() {}
    virtual void OnEnter() {}
    virtual void OnBackspace() {}
    virtual void OnTextInput(char character) {}
};

void MppInputQuit();
void MppInputAddHandler(const std::shared_ptr<MppInputHandler>& handler);
void MppInputRemoveHandler(MppInputHandler* handler);
void MppInputUpdate(uint64_t ticks);
void MppInputRender();
void MppInputHandle(SDL_Event* event);
