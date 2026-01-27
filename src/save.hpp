#pragma once

#include <cstdint>
#include <string>

// TODO: move into world
bool MppSaveInit();
void MppSaveQuit();
bool MppSaveIsExisting();
uint64_t MppSaveGetTicks();
void MppSaveUpdate(uint64_t ticks, bool force);
void MppSaveAdd(std::shared_ptr<SavepointEntity>& entity, int level);
void MppSaveRemove(const std::shared_ptr<SavepointEntity>& entity);