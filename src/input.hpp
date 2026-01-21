#pragma once

#include <cstdint>
#include <memory>

class MppInputHandler
{
public:
    virtual void OnAction() {}
    virtual void OnUp() {}
    virtual void OnDown() {}
    virtual void OnLeft() {}
    virtual void OnRight() {}
    virtual void OnHeldUp() {}
    virtual void OnHeldDown() {}
    virtual void OnHeldLeft() {}
    virtual void OnHeldRight() {}
};

void MppInputSetPlayer(const std::shared_ptr<MppInputHandler>& handler);
void MppInputSetInteraction(const std::shared_ptr<MppInputHandler>& handler);
void MppInputUpdate(uint64_t ticks);
