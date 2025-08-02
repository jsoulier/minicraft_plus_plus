#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <limits>

#include "camera.hpp"
#include "log.hpp"
#include "ticks.hpp"
#include "transform.hpp"

static constexpr glm::vec3 kUp{0.0f, 1.0f, 0.0f};
static constexpr float kEpsilon = std::numeric_limits<float>::epsilon();

MppCamera::MppCamera()
    : Mode{MppCameraMode::Perspective}
    , Position{}
    , Pitch{glm::radians(-45.0f)}
    , Yaw{glm::radians(90.0f)}
    , Width{1.0f}
    , Height{1.0f}
    , Fov{glm::radians(60.0f)}
    , Near{0.1f}
    , Far{1000.0f}
    , Distance{100.0f}
    , Speed{1.0f}
    , ViewProjMatrix{}
    , ViewMatrix{}
    , ProjMatrix{} {}

void MppCamera::Update(const MppTransform& transform, const MppTicks& ticks)
{
    glm::vec3 forward;
    forward.x = std::cosf(Yaw) * std::cosf(Pitch);
    forward.y = std::sinf(Pitch);
    forward.z = std::sinf(Yaw) * std::cosf(Pitch);
    glm::vec3 destination = transform.Position - forward * Distance;
    Position = glm::mix(Position, destination, Speed /* TODO: ticks.GetDeltaTime() */);
    ViewMatrix = glm::lookAt(Position, Position + forward, kUp);
    ProjMatrix = glm::perspective(Fov, Width / Height, Near, Far);
    ViewProjMatrix = ProjMatrix * ViewMatrix;
}

void MppCamera::SetMode(MppCameraMode mode)
{
    Mode = mode;
}

void MppCamera::SetViewport(float width, float height)
{
    Width = std::max(width, 1.0f);
    Height = std::max(height, 1.0f);
    if (width < kEpsilon || height < kEpsilon)
    {
        MppLog("Bad viewport: %f, %f", width, height);
    }
}

void MppCamera::SetPitch(float pitch)
{
    Pitch = pitch;
}

void MppCamera::SetYaw(float yaw)
{
    Yaw = yaw;
}

void MppCamera::SetFov(float fov)
{
    Fov = fov;
}

void MppCamera::SetNear(float near)
{
    Near = near;
}

void MppCamera::SetFar(float far)
{
    Far = far;
}

void MppCamera::SetDistance(float distance)
{
    Distance = distance;
}

void MppCamera::SetSpeed(float speed)
{
    Speed = speed;
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