#include <MyGL/Camera.hpp>

namespace my
{
    Camera::Camera() noexcept : m_position(0.0f, 0.0f, 3.0f), m_front(0.0f, 0.0f, -1.0f), m_up(0.0f, 1.0f, 0.0f), m_speed(50.0f) {}

    Camera::Camera(int x, int y) noexcept : m_position(x, y, 3.0f), m_front(0.0f, 0.0f, -1.0f), m_up(0.0f, 1.0f, 0.0f), m_speed(50.0f) {}

    void Camera::setPosition(int x, int y) noexcept {
        m_position = glm::vec3(x, y, 3.0f);
    }

    glm::vec2 Camera::getPosition() const noexcept {
        return glm::vec2(m_position.x, m_position.y);
    }

    void Camera::setSpeed(float speed) noexcept {
        m_speed = speed;
    }

    float Camera::getSpeed() const noexcept {
        return m_speed;
    }

    void Camera::moveUp(float frametime) noexcept {
        m_position += m_up * m_speed * frametime;
    }

    void Camera::moveDown(float frametime) noexcept {
        m_position -= m_up * m_speed * frametime;
    }

    void Camera::moveLeft(float frametime) noexcept {
        m_position -= glm::normalize(glm::cross(m_front, m_up)) * m_speed * frametime;
    }

    void Camera::moveRight(float frametime) noexcept {
        m_position += glm::normalize(glm::cross(m_front, m_up)) * m_speed * frametime;
    }

    glm::mat4 Camera::lookAt() const {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }
} // namespace my