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
    class Camera
    {
    private:
        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        float m_speed;
        // float m_roll; TODO

    public:
        /**
         * @brief Default constructor, creates a camera pointing to the center of
         *        the window
         */
        MYGL_EXPORT Camera() noexcept;

        /**
         * @brief Creates a 2D camera with its bottom left hand corner coordinates
         * @param x The camera's x coordinate
         * @param y The camera's y coordinate
         */
        MYGL_EXPORT Camera(int x, int y) noexcept;

        /**
         * @brief Sets the camera's position
         * @param x The x coordinate of the bottom left hand corner
         * @param y The y coordinate of the bottom left hand corner
         */
        MYGL_EXPORT void setPosition(int x, int y) noexcept;

        /**
         * @brief Get the camera's current position
         * @return The camera's bottom left hand corner's position
         */
        MYGL_EXPORT glm::vec2 getPosition() const noexcept;

        /**
         * @brief Sets the camera's speed, in pixels per second
         * @param speed The camera's speed, used with the frametime to produce
         *              fluid movements
         */
        MYGL_EXPORT void setSpeed(float speed) noexcept;

        /**
         * @brief Get the camera's speed
         * @return the camera's speed
         */
        MYGL_EXPORT float getSpeed() const noexcept;

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
        MYGL_EXPORT void moveUp(float frametime) noexcept;

        /**
         * @brief Moves the camera downwards
         */
        MYGL_EXPORT void moveDown(float frametime) noexcept;

        /**
         * @brief Moves the camera to the left
         */
        MYGL_EXPORT void moveLeft(float frametime) noexcept;

        /**
         * @brief Moves the camera to the right
         */
        MYGL_EXPORT void moveRight(float frametime) noexcept;

        /**@}*/

        // void rotate(float angle); TODO
        // void setRotation(float angle); TODO
        // float getRotation() const; TODO

        /**
         * @brief Returns the camera's "look_at" matrix, it is used internally
         *        by a Window to set its view when drawing things
         * @return The camera's "look_at" matrix
         */
        MYGL_EXPORT glm::mat4 lookAt() const;
    };

} // namespace my

#endif // MYGL_CAMERA