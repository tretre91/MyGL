#include <MyGL/Camera/FixedCamera.hpp>
using namespace my;

FixedCamera::FixedCamera() : m_position(0.0f, 0.0f, 3.0f), m_front(0.0f, 0.0f, -1.0f), m_up(0.0f, 1.0f, 0.0f) {}
FixedCamera::FixedCamera(glm::vec3 position) : m_position(position), m_front(0.0f, 0.0f, -1.0f), m_up(0.0f, 1.0f, 0.0f) {}
FixedCamera::FixedCamera(glm::vec3 position, glm::vec3 front, glm::vec3 up) : m_position(position), m_front(front), m_up(up) {}

void FixedCamera::setPosition(glm::vec3 position) {
	m_position = position;
}

glm::vec3 FixedCamera::getPosition() const {
	return m_position;
}

void FixedCamera::setFront(glm::vec3 front) {
	m_front = front;
}

glm::vec3 FixedCamera::getFront() const {
	return m_front;
}

void FixedCamera::setUp(glm::vec3 up) {
	m_up = up;
}

glm::vec3 FixedCamera::getUp() const {
	return m_up;
}

glm::mat4 FixedCamera::lookAt() const {
	return glm::lookAt(m_position, m_position + m_front, m_up);
}