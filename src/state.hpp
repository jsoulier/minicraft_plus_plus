#pragma once

enum class MppState
{
    Init,
    Play,
    Quit,
};

bool MppStateInit();
void MppStateQuit();
bool MppStateTick();