#pragma once

#include <savepoint/savepoint.hpp>

#include <array>
#include <memory>
#include <vector>

#include <minicraft++/tile.hpp>

class MppEntity;

class MppLevelSaveData
{
public:
    Savepoint Handle;
    SavepointStatus Status;
    std::unordered_map<SavepointID, std::weak_ptr<MppEntity>> References;
};

class MppLevel : public SavepointPoly
{
public:
    static constexpr int kSize = 256;

    MppLevel();
    virtual void Generate();
    void Load();
    void Visit(SavepointVisitor& visitor) override {}
    void Update(uint64_t ticks);
    void Render() const;
    void Save();
    MppTile& GetTile(int x, int y);
    void SetTile(const MppTile& tile, int x, int y);
    std::vector<std::shared_ptr<MppEntity>> GetEntities();
    std::vector<std::shared_ptr<MppEntity>> GetEntities(int x, int y);
    void AddEntity(std::shared_ptr<MppEntity>& entity);
    void RemoveEntity(std::shared_ptr<MppEntity>& entity);
    void SetSaveData(const std::shared_ptr<MppLevelSaveData>& state);
    void SetLevel(int level);
    virtual int GetLightColor() const;

protected:
    virtual MppTile GenerateTile(float x, float y) const = 0;

private:
    std::vector<std::shared_ptr<MppEntity>> Entities;
    std::array<std::array<MppTile, kSize>, kSize> Tiles;
    std::shared_ptr<MppLevelSaveData> SaveData;
    int SaveTileX1;
    int SaveTileY1;
    int SaveTileX2;
    int SaveTileY2;
    int Level;
};
