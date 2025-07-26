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

    MppCameraMode Mode;
    float Pitch;
    float Yaw;
    float Fov;
    float Near;
    float Far;
    float Distance;
    float Speed;
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
    MppCameraMode Mode;
    glm::vec3 Position;
    float Pitch;
    float Yaw;
    float Width;
    float Height;
    float Fov;
    float Near;
    float Far;
    float Distance;
    float Speed;
    glm::mat4 ViewProjMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ProjMatrix;
};