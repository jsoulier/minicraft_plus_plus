#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>

#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/item.hpp>

class MppCampfireEntity : public MppFurnitureEntity
{
    SAVEPOINT_POLY(MppCampfireEntity)

public:
    void Render() const override;
    MppItemID GetItemID() const override;
    
private:
    int GetAnimationTickRate() const override;
    int GetAnimationPose1X() const override;
    int GetAnimationPose1Y() const override;
    int GetAnimationPose2X() const override;
    int GetAnimationPose2Y() const override;
    int GetAnimationPose3X() const override;
    int GetAnimationPose3Y() const override;
    int GetAnimationPose4X() const override;
    int GetAnimationPose4Y() const override;
    int GetColor1() const override;
    int GetColor2() const override;
    int GetColor3() const override;
    int GetColor4() const override;
    int GetColor5() const override;
};
