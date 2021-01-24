#ifndef MY_FIXED_CAMERA
#define MY_FIXED_CAMERA

#include "../Header.hpp"

namespace my 
{
	
	class FixedCamera {

	protected:
		glm::vec3 m_position;
		glm::vec3 m_front;
		glm::vec3 m_up;

	public:
		FixedCamera();
		FixedCamera(glm::vec3 position);
		FixedCamera(glm::vec3 position, glm::vec3 front, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

		void setPosition(glm::vec3 position);
		glm::vec3 getPosition() const;

		void setFront(glm::vec3 front);
		glm::vec3 getFront() const;

		void setUp(glm::vec3 up);
		glm::vec3 getUp() const;

		glm::mat4 lookAt() const;
	};

}


#endif //MY_FIXED_CAMERA