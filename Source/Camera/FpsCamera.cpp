#include <MyGL/Camera/FpsCamera.hpp>
using namespace my;

FpsCamera::FpsCamera() : Cam3D() {}
FpsCamera::FpsCamera(glm::vec3 position, glm::vec3 front) : Cam3D(position, front) {}

void FpsCamera::moveFront(float frametime) {
    m_position += glm::vec3(m_front.x, 0.0f, m_front.z) * m_speed * frametime;
}

void FpsCamera::moveBack(float frametime) {
    m_position -= glm::vec3(m_front.x, 0.0f, m_front.z) * m_speed * frametime;
}