#ifndef MY_FPS_CAMERA
#define MY_FPS_CAMERA

#include "Cam3D.hpp"

namespace my 
{
	
	class FpsCamera : public Cam3D {

	private:


	public:
		FpsCamera();
		FpsCamera(glm::vec3 position, glm::vec3 front);

		void moveFront(float frametime);
		void moveBack(float frametime);
	};

}

#endif //MY_FPS_CAMERA