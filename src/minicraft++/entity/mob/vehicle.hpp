#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/entity/mob/mob.hpp>

class MppMobEntityMount;
enum MppTilePhysicsType : uint8_t;

class MppVehicleEntity : public MppMobEntity
{
public:
    MppVehicleEntity();
    void OnAdd() override;
    void Visit(SavepointVisitor& visitor) override;
    void Update(uint64_t ticks) override;
    void Render() const override;
    bool OnAction(std::shared_ptr<MppEntity>& instigator) override;
    bool OnInteraction(std::shared_ptr<MppEntity>& instigator) override;
    virtual MppEntityCollision OnCollision(std::shared_ptr<MppEntity>& instigator, int dx, int dy);
    void DoAction() override;
    bool Drop(const std::shared_ptr<MppEntity>& instigator) override;
    virtual MppTilePhysicsType GetPhysicsType() const = 0;

private:
    std::shared_ptr<MppMobEntityMount> Mount;
};
