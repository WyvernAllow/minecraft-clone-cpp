#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fov, float aspect, float near, float far)
    : position(0), fov(fov), aspect(aspect), near(near), far(far), pitch(0), yaw(0)
{
    Update();
}

void Camera::Update()
{
    pitch = glm::clamp(pitch, -89.99f, 89.99f);

    glm::vec3 forward;
    forward.x = glm::cos(glm::radians(pitch)) * sinf(glm::radians(yaw));
    forward.y = glm::sin(glm::radians(pitch));
    forward.z = glm::cos(glm::radians(pitch)) * cosf(glm::radians(yaw));
    
    m_forward = glm::normalize(forward);
    m_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_forward));
    m_up = glm::normalize(glm::cross(m_forward, m_right));

    m_view = glm::lookAt(position, position + m_forward, m_up);
    m_proj = glm::perspective(glm::radians(fov), aspect, near, far);
}