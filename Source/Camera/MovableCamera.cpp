#include <MyGL/Camera/MovableCamera.hpp>
using namespace my;

MovableCamera::MovableCamera() : m_speed(5.0f), FixedCamera() {}
MovableCamera::MovableCamera(glm::vec3 position) : m_speed(5.0f), FixedCamera(position) {}
MovableCamera::MovableCamera(glm::vec3 position, glm::vec3 front, glm::vec3 up) : m_speed(5.0f), FixedCamera(position, front, up) {}

void MovableCamera::setSpeed(float speed) {
	m_speed = speed;
}

float MovableCamera::getSpeed() const {
	return m_speed;
}