#pragma once

#include <memory>
#include <vector>

class MppEntity;
class MppTile;

bool MppWorldInit();
void MppWorldQuit();
void MppWorldUpdate(uint64_t ticks);
void MppWorldRender();
MppTile& MppWorldGetTile(int x, int y, int level);
MppTile& MppWorldGetTile(int x, int y);
std::vector<std::shared_ptr<MppEntity>>& MppWorldGetEntities(int level);
std::vector<std::shared_ptr<MppEntity>>& MppWorldGetEntities();
void MppWorldSetTile(const MppTile& tile, int x, int y, int level);
void MppWorldSetTile(const MppTile& tile, int x, int y);
void MppWorldAddEntity(const std::shared_ptr<MppEntity>& entity, int level);
void MppWorldAddEntity(const std::shared_ptr<MppEntity>& entity);
int MppWorldGetLevel();
int MppWorldGetLevelCount();
int MppWorldGetSize();
