#pragma once

#include "input.hpp"

enum class MppState
{
    Init,
    Play,
    Quit,
};

bool MppStateInit();
void MppStateQuit();
bool MppStateTick();
MppState MppStateGet();
MppInputType MppStateGetInputType();
float MppStateGetInput(MppInput input);