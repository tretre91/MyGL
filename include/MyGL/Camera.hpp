#ifndef MYGL_CAMERA
#define MYGL_CAMERA

#include "mygl_export.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace my
{
    /**
     * @brief Class used to manipulate a 2D camera
     */
    class MYGL_EXPORT Camera
    {
    private:
        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        float m_speed;
        float m_roll;

    public:
        /**
         * @brief Default constructor, creates a camera pointing to the center of
         *        the window
         */
        Camera();

        /**
         * @brief Creates a 2D camera with its bottom left hand corner coordinates
         * @param x The camera's x coordinate
         * @param y The camera's y coordinate
         */
        Camera(int x, int y);

        /**
         * @brief Sets the camera's position
         * @param x The x coordinate of the bottom left hand corner
         * @param y The y coordinate of the bottom left hand corner
         */
        void setPosition(int x, int y);

        /**
         * @brief Get the camera's current position
         * @return The camera's bottom left hand corner's position
         */
        glm::vec2 getPosition() const;

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

        /**
         * @name Functions related to the camera's movement
         *
         * These functions all have a common parameter (float frametime), this
         * parameter represents the frametime, it is needed in order to move the
         * camera at a constant speed (speed which is expressed in pixels / second
         * and defined with a call to setSpeed(float)), the speed would then be
         * independent from the framerate.
         * You can get the frametime with a call to the getFrametime() function of the
         * current Window, for example :
         * ```c++
         *
         * my::Window window(800, 600, "window");
         *
         * my::Camera camera(0, 0);
         * camera.setSpeed(50.0f);
         * window.setCamera(camera);
         *
         * ...
         *
         * float frametime;
         *
         * bool running = true;
         * while(window.isRunning()) {
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

        // void rotate(float angle); TODO
        // void setRotation(float angle); TODO
        // float getRotation() const; TODO

        /**
         * @brief Returns the camera's "look_at" matrix, it is used internally
         *        by a Window to set its view when drawing things
         * @return The camera's "look_at" matrix
         */
        glm::mat4 lookAt() const;
    };

} // namespace my

#endif // MYGL_CAMERA