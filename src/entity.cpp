#include <SDL3/SDL.h>

#include <memory>

#include "entity.hpp"
#include "id.hpp"
#include "mob_entity.hpp"
#include "model.hpp"
#include "player_entity.hpp"
#include "transform.hpp"

MppEntity::MppEntity()
    : Type{MppEntityType::Null}
    , Transform{}
    , Id{}
    , Model{MppModelDefault} {}

std::shared_ptr<MppEntity> MppEntity::Create(MppEntityType type, void* args)
{
    std::shared_ptr<MppEntity> entity;
    switch (type)
    {
    case MppEntityType::Player:
        entity = std::make_shared<MppPlayerEntity>();
        break;
    }
    if (!entity)
    {
        SDL_Log("Failed to create entity: %d", type);
        return {};
    }
    entity->Type = type;
    return entity;
}

void MppEntity::SetId(MppId id)
{
    Id = id;
}

MppEntityType MppEntity::GetType() const
{
    return Type;
}

const MppTransform& MppEntity::GetTransform() const
{
    return Transform;
}

MppId MppEntity::GetId() const
{
    return Id;
}

MppModel MppEntity::GetModel() const
{
    return Model;
}