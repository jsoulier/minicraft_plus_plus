#pragma once

#include <glm/glm.hpp>

class MppTime;
class MppTransform;

enum class MppCameraMode
{
    Perspective,
    Ortho,
};

struct MppCameraSettings
{
    MppCameraSettings();

    MppCameraMode mode;
    float pitch;
    float yaw;
    float fov;
    float near;
    float far;
    float speed;
};

class MppCamera
{
public:
    MppCamera(const MppCameraSettings& settings = {});
    void Update(const MppTransform& target, const MppTime& time);
    void SetViewport(float width, float height);
    const glm::mat4& GetViewProjMatrix() const;
    const glm::mat4& GetViewMatrix() const;
    const glm::mat4& GetProjMatrix() const;

private:
    MppCameraMode mode;
    glm::vec3 position;
    float pitch;
    float yaw;
    float width;
    float height;
    float fov;
    float near;
    float far;
    float speed;
    glm::mat4 viewProjMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;
};