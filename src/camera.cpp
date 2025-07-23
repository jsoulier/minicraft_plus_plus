#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "camera.hpp"
#include "transform.hpp"

MppCameraSettings::MppCameraSettings()
    : mode{MppCameraMode::Perspective}
    , pitch{0.0f}
    , yaw{0.0f}
    , fov{glm::radians(60.0f)}
    , near{0.01f}
    , far{1000.0f}
    , speed{1.0f} {}

MppCamera::MppCamera(const MppCameraSettings& settings)
    : mode{settings.mode}
    , pitch{settings.pitch}
    , yaw{settings.yaw}
    , width{1.0f}
    , height{1.0f}
    , fov{settings.yaw}
    , near{settings.near}
    , far{settings.far}
    , speed{settings.speed} {}

void MppCamera::Update(const MppTransform& target, const MppTime& time)
{

}

void MppCamera::SetViewport(float width, float height)
{
    this->width = width;
    this->height = height;
}

const glm::mat4& MppCamera::GetViewProjMatrix() const
{
    return viewProjMatrix;
}

const glm::mat4& MppCamera::GetViewMatrix() const
{
    return viewMatrix;
}

const glm::mat4& MppCamera::GetProjMatrix() const
{
    return projMatrix;
}