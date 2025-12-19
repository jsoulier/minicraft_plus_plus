#include <SDL3/SDL.h>

#include <memory>

#include "controller.hpp"
#include "level.hpp"
#include "mob_entity.hpp"
#include "version.hpp"

MppMobEntity::MppMobEntity()
    : MppEntity()
    , DirectionX{0}
    , DirectionY{1}
    , DrawInventory{false}
    , Controller{}
    , Inventory{}
{
}

void MppMobEntity::Update(MppLevel& level, MppRenderer& renderer, int ticks)
{
    SDL_assert(DirectionX || DirectionY);
    Controller->Update(level, ticks);
    SDL_assert(DirectionX || DirectionY);
    MppEntity::Update(level, renderer, ticks);
    if (DrawInventory)
    {
        Inventory.Draw(renderer);
    }
}

void MppMobEntity::Visit(SavepointVisitor& visitor)
{
    MppEntity::Visit(visitor);
    visitor(Inventory);
}

void MppMobEntity::Move(MppLevel& level, int dx, int dy, int ticks)
{
    SDL_assert(dx || dy);
    MppEntity::Move(level, dx, dy);
    DirectionX = dx;
    DirectionY = dy;
}

int MppMobEntity::GetSize() const
{
    return 16;
}

void MppMobEntity::SetDrawInventory(bool draw)
{
    DrawInventory = draw;
}

bool MppMobEntity::GetDrawInventory() const
{
    return DrawInventory;
}

void MppMobEntity::SetController(const std::shared_ptr<MppController>& controller)
{
    Controller = controller;
}
