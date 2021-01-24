#include <MyGL/Camera/Cam3D.hpp>
using namespace my;

Cam3D::Cam3D() : m_sensitivity(0.1f), m_pitch(0.0f), m_yaw(-90.0f), MovableCamera() {}
Cam3D::Cam3D(glm::vec3 position) : m_sensitivity(0.1f), m_pitch(0.0f), m_yaw(-90.0f), MovableCamera(position) {}
Cam3D::Cam3D(glm::vec3 position, glm::vec3 front, glm::vec3 up) : m_sensitivity(0.1f), m_pitch(0.0f), m_yaw(-90.0f), MovableCamera(position, front, up) {}

void Cam3D::moveUp(float frametime) {
	m_position += m_up * m_speed * frametime;
}

void Cam3D::moveDown(float frametime) {
	m_position -= m_up * m_speed * frametime;
}

void Cam3D::moveFront(float frametime) {
	m_position += m_front * m_speed * frametime;
}

void Cam3D::moveBack(float frametime) {
	m_position -= m_front * m_speed * frametime;
}

void Cam3D::moveLeft(float frametime) {
	m_position -= glm::normalize(glm::cross(m_front, m_up)) * m_speed * frametime;
}

void Cam3D::moveRight(float frametime) {
	m_position += glm::normalize(glm::cross(m_front, m_up)) * m_speed * frametime;
}

void Cam3D::moveView(float pitch, float yaw) {
	m_pitch -= pitch * m_sensitivity;
	if (m_pitch > 89.0f) pitch = 89.0f;
	else if (m_pitch < -89.0f) pitch = -89.0f;
	m_yaw += yaw * m_sensitivity;
	
	m_front = glm::normalize(glm::vec3(
		glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch)), 
		glm::sin(glm::radians(m_pitch)),
		glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch))
	));
}

void Cam3D::setSensitivity(float sensi) {
	m_sensitivity = sensi;
}

float Cam3D::getSensitivity() const {
	return m_sensitivity;
}