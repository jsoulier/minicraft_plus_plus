#pragma once

#include <SDL3/SDL.h>

#include <cstdint>
#include <memory>

class MppInputHandler
{
public:
    virtual void OnInputGainFocus() {}
    virtual void OnInputLoseFocus() {}
    virtual void OnInputUpdate(uint64_t ticks) {}
    virtual void OnInputRender() const {}
    virtual void OnInputHeldUp() {}
    virtual void OnInputHeldDown() {}
    virtual void OnInputHeldLeft() {}
    virtual void OnInputHeldRight() {}
    virtual void OnInputAction() {}
    virtual void OnInputDrop() {}
    virtual void OnInputInteract();
    virtual void OnInputDismount() {}
    virtual void OnInputExit();
    virtual void OnInputUp() {}
    virtual void OnInputDown() {}
    virtual void OnInputLeft() {}
    virtual void OnInputRight() {}
    virtual void OnInputUpArrow() {}
    virtual void OnInputDownArrow() {}
    virtual void OnInputLeftArrow() {}
    virtual void OnInputRightArrow() {}
    virtual void OnInputEnter() {}
    virtual void OnInputBackspace() {}
    virtual void OnInputText(char character) {}
};

void MppInputAddHandler(const std::shared_ptr<MppInputHandler>& handler);
void MppInputRemoveHandler(MppInputHandler* handler);
void MppInputUpdate(uint64_t ticks);
void MppInputRender();
void MppInputHandle(SDL_Event* event);
