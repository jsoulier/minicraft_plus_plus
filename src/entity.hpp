#pragma once

#include <memory>

#include "id.hpp"
#include "model.hpp"
#include "transform.hpp"

class MppTicks;

enum class MppEntityType
{
    Null,
    Player,
};

class MppEntity
{
protected:
    MppEntity();

public:
    static std::shared_ptr<MppEntity> Create(MppEntityType type, void* args = nullptr);
    virtual void Update(const MppTicks& ticks) = 0;
    void SetId(MppId id);
    MppEntityType GetType() const;
    const MppTransform& GetTransform() const;
    MppId GetId() const;
    MppModel GetModel() const;

protected:
    MppEntityType Type;
    MppTransform Transform;
    MppId Id;
    MppModel Model;
};