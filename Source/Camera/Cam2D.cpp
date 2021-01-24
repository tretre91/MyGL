#include <MyGL/Camera/Cam2D.hpp>
using namespace my;

Cam2D::Cam2D() : roll(0.0f), MovableCamera() {}
Cam2D::Cam2D(int x, int y) : roll(0.0f), MovableCamera(glm::vec3((float)x, (float)y, 3.0f)) {}

void Cam2D::moveUp(float frametime) {
	m_position += m_up * m_speed * frametime;
}

void Cam2D::moveDown(float frametime) {
	m_position -= m_up * m_speed * frametime;
}

void Cam2D::moveLeft(float frametime) {
	m_position -= glm::normalize(glm::cross(m_front, m_up)) * m_speed * frametime;
}

void Cam2D::moveRight(float frametime) {
	m_position += glm::normalize(glm::cross(m_front, m_up)) * m_speed * frametime;
}