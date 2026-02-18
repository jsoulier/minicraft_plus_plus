#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/entity/mob/vehicle.hpp>

class MppBoatEntity final : public MppVehicleEntity
{
    SAVEPOINT_POLY(MppBoatEntity)

public:
    MppItemID GetItemID() const override;

protected:
    MppTilePhysicsType GetPhysicsType() const override;
    int GetAnimationPose1X() const override;
    int GetAnimationPose1Y() const override;
    int GetColor1() const override;
    int GetColor2() const override;
    int GetColor3() const override;
    int GetColor4() const override;
    int GetColor5() const override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetMoveTickRate() const override;
    int GetSpeed() const override;
};
