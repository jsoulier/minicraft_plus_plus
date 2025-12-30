#include <SDL3/SDL.h>
#include <savepoint/visitor.hpp>

#include <memory>

#include "controller.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include "mob_entity.hpp"
#include "version.hpp"

MppMobEntity::MppMobEntity()
    : MppEntity()
    , DeltaX{0}
    , DeltaY{1}
    , Controller{}
    , Inventory{}
{
}

void MppMobEntity::Update(MppLevel& level, MppRenderer& renderer, int ticks)
{
    SDL_assert(DeltaX || DeltaY);
    Controller->Update(level, renderer, ticks);
    SDL_assert(DeltaX || DeltaY);
    MppEntity::Update(level, renderer, ticks);
}

void MppMobEntity::Visit(SavepointVisitor& visitor)
{
    MppEntity::Visit(visitor);
    visitor(DeltaX);
    visitor(DeltaY);
    if (Inventory)
    {
        visitor(*Inventory);
    }
}

void MppMobEntity::Move(MppLevel& level, int dx, int dy, int ticks)
{
    SDL_assert(dx || dy);
    MppEntity::Move(level, dx, dy);
    DeltaX = dx;
    DeltaY = dy;
}

int MppMobEntity::GetSize() const
{
    return 16;
}

const std::shared_ptr<MppInventory>& MppMobEntity::GetInventory()
{
    return Inventory;
}

void MppMobEntity::SetController(const std::shared_ptr<MppController>& controller)
{
    Controller = controller;
}

void MppMobEntity::SetInventory(const std::shared_ptr<MppInventory>& inventory)
{
    Inventory = inventory;
}
