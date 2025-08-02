#include <glm/glm.hpp>

#include "input.hpp"
#include "player_entity.hpp"
#include "state.hpp"

static constexpr float kEpsilon = std::numeric_limits<float>::epsilon();

glm::vec2 MppPlayerEntity::GetPositionDelta() const
{
    glm::vec2 delta{};
    delta.y += MppStateGetInput(MppInput::MoveNorth);
    delta.y -= MppStateGetInput(MppInput::MoveSouth);
    delta.x += MppStateGetInput(MppInput::MoveEast);
    delta.x -= MppStateGetInput(MppInput::MoveWest);
    float length = glm::length(delta);
    if (length < kEpsilon)
    {
        return {};
    }
    if (MppStateGetInputType() == MppInputType::KeyboardMouse)
    {
        delta /= length;
    }
    return delta;
}