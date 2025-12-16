#include <cmath>
#include <limits>

#include "color.hpp"
#include "console.hpp"
#include "controller.hpp"
#include "level.hpp"
#include "mob_entity.hpp"
#include "renderer.hpp"
#include "sprite.hpp"

MppMobEntity::MppMobEntity()
    : MppEntity()
{
}

void MppMobEntity::Update(MppLevel& level, MppRenderer& renderer, int ticks)
{
    Controller->Update(level, ticks);
    MppEntity::Update(level, renderer, ticks);

    // TODO: remove
    int x = GetPhysicsX();
    int y = GetPhysicsY();
    int width = GetPhysicsWidth();
    int height = GetPhysicsHeight();
    renderer.DrawRect(kMppColorWhite, x, y, width, height, MppRenderer::LayerMobEntity);
}

void MppMobEntity::Move(MppLevel& level, int dx, int dy, int ticks)
{
    MppEntity::Move(level, dx, dy);
}

int MppMobEntity::GetSize() const
{
    return 16;
}

void MppMobEntity::SetController(const std::shared_ptr<MppController>& controller)
{
    Controller = controller;
}
