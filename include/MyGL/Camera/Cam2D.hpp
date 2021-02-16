#ifndef MY_2D_CAMERA
#define MY_2D_CAMERA

#include "MovableCamera.hpp"

namespace my 
{
    /**
     * @brief Class used to manipulate a 2D camera
     * 
    */
    class MYGL_EXPORT Cam2D : public MovableCamera {

    private:
        float roll;

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
         * In order to compute the frametime you could use an sf::Clock object which is
         * restarted each frame like this :
         * ```c++
         * // window initialization, etc
         * ...
         *
         * sf::Clock clk;
         * float frametime;
         *
         * bool running = true;
         * while(running) {
         *   ...
         *   window.clear(...);
         *
         *   frametime = clk.restart().asSeconds();
         *   // Do some interesting things with the result
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