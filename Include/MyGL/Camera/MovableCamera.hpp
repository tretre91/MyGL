#ifndef MY_MOVABLE_CAMERA
#define MY_MOVABLE_CAMERA

#include "FixedCamera.hpp"

namespace my 
{
	/**
	 * @brief An abstract class which extends FixedCamera to allow the camera to
	 *        be moved, inherited by all moving cameras
	*/
	class MovableCamera : public FixedCamera {

	protected:
		float m_speed;

	public:
		/**
		 * @brief Default constructor, initializes the camera's speed at 5 px/s
		*/
		MovableCamera();

		/**
		 * @brief Initializes the camera with an initial position and a speed of
		 *        5 px/s
		 * 
		 * @param position The initial position (in 3D) of the camera
		*/
		MovableCamera(glm::vec3 position);

		/**
		 * @brief Initializes the camera's position, where it points at and it's 
		 *        orientation
		 * 
		 * @param position The camera's initial position
		 * @param front The point in space the camera is facing
		 * @param up The camera's up vector
		*/
		MovableCamera(glm::vec3 position, glm::vec3 front, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

		virtual void moveUp(float frametime) = 0;
		virtual void moveDown(float frametime) = 0;

		/**
		 * @brief Sets the camera's speed, in pixels per second
		 * @param speed The camera's speed, used with the frametime to produce
		 *              fluid movements
		*/
		void setSpeed(float speed);

		/**
		 * @brief Get the camera's speed
		 * @return the camera's speed
		*/
		float getSpeed() const;

	};

}

#endif //MY_MOVABLE_CAMERA