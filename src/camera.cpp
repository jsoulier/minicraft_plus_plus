#include <SDL3/SDL.h>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <limits>

#include "camera.hpp"
#include "time.hpp"
#include "transform.hpp"

static constexpr glm::vec3 kUp{0.0f, 1.0f, 0.0f};
static constexpr float kEpsilon = std::numeric_limits<float>::epsilon();

MppCameraSettings::MppCameraSettings()
    : Mode{MppCameraMode::Perspective}
    , Pitch{glm::radians(0.0f)}
    , Yaw{glm::radians(0.0f)}
    , Fov{glm::radians(60.0f)}
    , Near{0.01f}
    , Far{1000.0f}
    , Distance{100.0f}
    , Speed{1.0f} {}

MppCamera::MppCamera(const MppCameraSettings& settings)
    : Mode{settings.Mode}
    , Pitch{settings.Pitch}
    , Yaw{settings.Yaw}
    , Width{1.0f}
    , Height{1.0f}
    , Fov{settings.Yaw}
    , Near{settings.Near}
    , Far{settings.Far}
    , Distance{settings.Distance}
    , Speed{settings.Speed} {}

void MppCamera::Update(const MppTransform& target, const MppTime& time)
{
    glm::vec3 forward;
    forward.x = std::cosf(Yaw) * std::cosf(Pitch);
    forward.y = std::sinf(Pitch);
    forward.z = std::sinf(Yaw) * std::cosf(Pitch);
    glm::vec3 destination = target.Position - forward * Distance;
    Position = glm::mix(Position, destination, Speed * time.GetDeltaTime());
    ViewMatrix = glm::lookAt(Position, Position + forward, kUp);
    ProjMatrix = glm::perspective(Fov, Width / Height, Near, Far);
    ViewProjMatrix = ProjMatrix * ViewMatrix;
}

void MppCamera::SetViewport(float width, float height)
{
    Width = std::max(width, 1.0f);
    Height = std::max(height, 1.0f);
    if (width < kEpsilon || height < kEpsilon)
    {
        SDL_Log("Bad viewport: %f, %f", width, height);
    }
}

const glm::mat4& MppCamera::GetViewProjMatrix() const
{
    return ViewProjMatrix;
}

const glm::mat4& MppCamera::GetViewMatrix() const
{
    return ViewMatrix;
}

const glm::mat4& MppCamera::GetProjMatrix() const
{
    return ProjMatrix;
}