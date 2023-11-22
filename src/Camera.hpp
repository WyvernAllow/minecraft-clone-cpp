#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera
{
public:
    glm::vec3 position;
    float pitch;
    float yaw;

    float fov;
    float aspect;
    float near;
    float far;

public:
    Camera(float fov, float aspect, float near = 0.001f, float far = 1000.0f);

    void Update();

    glm::vec3 GetForward() const {return m_forward;}
    glm::vec3 GetRight() const {return m_right;}
    glm::vec3 GetUp() const {return m_up;}

    const glm::mat4& GetView() const {return m_view;}
    const glm::mat4& GetProj() const {return m_proj;}

private:
    glm::vec3 m_forward;
    glm::vec3 m_right;
    glm::vec3 m_up;

    glm::mat4 m_view;
    glm::mat4 m_proj;
};

#endif // CAMERA_HPP