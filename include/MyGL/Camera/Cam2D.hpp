#ifndef MY_2D_CAMERA
#define MY_2D_CAMERA

#include "../mygl_export.h"
#include "MovableCamera.hpp"

namespace my 
{
    /**
     * @brief Class used to manipulate a 2D camera
     * 
    */
    class MYGL_EXPORT Cam2D : public MovableCamera {

    private:
        float m_roll;

    public:
        /**
         * @brief Default constructor, creates a camera pointing to the center of
         *        the window
        */
        Cam2D();

        /**
         * @brief Creates a 2D camera with its bottom left hand corner coordinates 
         * @param x The camera's x coordinate
         * @param y The camera's y coordinate
        */
        Cam2D(int x, int y);

        /**
         * @brief Sets the camera's position
         * @param x The x coordinate of the bottom left hand corner
         * @param y The y coordinate of the bottom left hand corner
        */
        void setPosition(int x, int y);

        /**
         * @name Functions related to the camera's movement
         * 
         * These functions all have a common parameter (float frametime), this
         * parameter represents the frametime, it is needed in order to move the
         * camera at a constant speed (speed which is expressed in pixels / second 
         * and defined with a call to setSpeed(float)), the speed would then be
         * independent from the framerate.
         * You can get the framtime with a call to the method getFrametime() of the
         * current GLWindow, for example :
         * ```c++
         * 
         * my::GLWindow window(800, 600, "window");
         * 
         * my::Cam2D camera(0, 0);
         * camera.setSpeed(50.0f);
         * window.setCamera(camera);
         * 
         * ...
         * 
         * float frametime;
         *
         * bool running = true;
         * while(running) {
         *   ...
         *   window.clear(...);
         *
         *   frametime = window.getFrametime();
         *   camera.moveUp(frametime);
         *   ...
         *
         *   window.display()
         * }
         * ```
        */
        /**@{*/
        
        /**
         * @brief Moves the camera upwards
        */
        void moveUp(float frametime);

        /**
         * @brief Moves the camera downwards
        */
        void moveDown(float frametime);

        /**
         * @brief Moves the camera to the left
        */
        void moveLeft(float frametime);

        /**
         * @brief Moves the camera to the right
        */
        void moveRight(float frametime);

        /**@}*/
    };

}

#endif //MY_2D_CAMERA