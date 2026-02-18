#include <cmath>
#include <cstdint>
#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/particle/emitter.hpp>
#include <minicraft++/entity/particle/hit.hpp>
#include <minicraft++/entity/particle/particle.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/random.hpp>
#include <minicraft++/world.hpp>

std::shared_ptr<MppParticleEntity> MppParticleEntityEmitterCreateHit()
{
    return std::make_shared<MppHitEntity>();
}

MppParticleEntityEmitter::MppParticleEntityEmitter()
    : Function{nullptr}
    , CooldownTime{0}
    , RandomTime{0}
    , NextTime{1}
{
}

void MppParticleEntityEmitter::Update(uint64_t ticks, int x, int y)
{
    MppAssert(Function);
    if (ticks % NextTime != 0)
    {
        return;
    }
    std::shared_ptr<MppParticleEntity> particle = Function();
    if (!particle)
    {
        MppLog("Failed to create particle");
        return;
    }
    particle->SetX(x);
    particle->SetY(y);
    std::shared_ptr<MppEntity> entity = particle->Cast<MppEntity>();
    MppWorldAddEntity(entity);
    NextTime = std::max(CooldownTime + MppRandomGetInt(0, RandomTime), 1);
}

void MppParticleEntityEmitter::SetFunction(MppParticleEntityEmitterFunction function)
{
    Function = function;
}

void MppParticleEntityEmitter::SetCooldownTime(int time)
{
    CooldownTime = time;
}

void MppParticleEntityEmitter::SetRandomTime(int time)
{
    RandomTime = time;
}
