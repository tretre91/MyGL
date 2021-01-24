#ifndef MY_MOVABLE_CAMERA
#define MY_MOVABLE_CAMERA

#include "FixedCamera.hpp"

namespace my 
{

	class MovableCamera : public FixedCamera {

	protected:
		float m_speed;

	public:
		MovableCamera();
		MovableCamera(glm::vec3 position);
		MovableCamera(glm::vec3 position, glm::vec3 front, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

		virtual void moveUp(float frametime) = 0;
		virtual void moveDown(float frametime) = 0;

		void setSpeed(float speed);
		float getSpeed() const;

	};

}

#endif //MY_MOVABLE_CAMERA