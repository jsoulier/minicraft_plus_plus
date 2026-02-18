#pragma once

#include <cstdint>
#include <memory>

class MppParticleEntity;

using MppParticleEntityEmitterFunction = std::shared_ptr<MppParticleEntity>(*)();

std::shared_ptr<MppParticleEntity> MppParticleEntityEmitterCreateHit();

class MppParticleEntityEmitter
{
public:
    MppParticleEntityEmitter();
    void Update(uint64_t ticks, int x, int y);
    void SetFunction(MppParticleEntityEmitterFunction function);
    void SetCooldownTime(int time);
    void SetRandomTime(int time);

private:
    MppParticleEntityEmitterFunction Function;
    int CooldownTime;
    int RandomTime;
    int NextTime;
};
