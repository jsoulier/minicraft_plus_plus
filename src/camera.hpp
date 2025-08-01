#pragma once

#include <glm/glm.hpp>

class MppTime;
class MppTransform;

enum class MppCameraMode
{
    Perspective,
    Ortho,
};

class MppCamera
{
public:
    MppCamera();
    void Update(const MppTransform& target, const MppTime& time);
    void SetMode(MppCameraMode mode);
    void SetViewport(float width, float height);
    void SetPitch(float pitch);
    void SetYaw(float yaw);
    void SetFov(float fov);
    void SetNear(float near);
    void SetFar(float far);
    void SetDistance(float distance);
    void SetSpeed(float speed);
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