#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>

#include <minicraft++/sprite.hpp>
#include <minicraft++/entity/mob/mob.hpp>

class MppCreatureEntity : public MppMobEntity
{
public:
    MppCreatureEntity();
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Update(uint64_t ticks) override;
    virtual bool OnAction(MppEntity& instigator) override;
    int GetMoveTickRate() const override;
    int GetDefaultSpeed() const;
    int GetFleeSpeed() const;
    int GetIdleCooldown() const;
    int GetFleeCooldown() const;
    int GetFleeTicks() const;

protected:
    std::shared_ptr<MppController> GetController() override;

private:
    int FleeTicks;
};
