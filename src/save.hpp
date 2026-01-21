#pragma once

#include <savepoint/time.hpp>

#include <cstdint>
#include <string>

bool MppSaveInit();
void MppSaveQuit();
bool MppSaveIsExisting();
uint64_t MppSaveGetTicks();
void MppSaveUpdate(uint64_t ticks, bool force);
