#pragma once

#include <memory>

#include "../entity.hpp"

class MppInventory;

class MppFurnitureEntity : public MppEntity
{
public:
    virtual void Render() override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetSize() const override;
};