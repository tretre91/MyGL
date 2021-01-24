#ifndef MY_2D_CAMERA
#define MY_2D_CAMERA

#include "MovableCamera.hpp"

namespace my 
{
	
	class Cam2D : public MovableCamera {

	private:
		float roll;

	public:
		Cam2D();
		Cam2D(int x, int y);

		void moveUp(float frametime);
		void moveDown(float frametime);
		void moveLeft(float frametime);
		void moveRight(float frametime);
	};

}

#endif //MY_2D_CAMERA