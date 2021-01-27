#ifndef MY_FIXED_CAMERA
#define MY_FIXED_CAMERA

#include "../Header.hpp"

namespace my 
{
    /**
     * @brief Base class for all other types of camera, defines a fixed camera
     *        which can be used for 2D or 3D
     * 
     *        __Usage in 2D__, for example a camera at the position (30, 50)
     *        would be initialized with :
     * 
     *            my::FixedCamera camera(glm::vec3(30.0f, 50.0f, 1.0f), glm::vec3(30.0f, 50.0f, -1.0f));
     * 
     *        When in 2D, the position's z coordinate __must__ be set to a strictly
     *        positive value (z > 0.0) in order to avoid clipping problems with
     *        shapes drawn to the window.
     *        Anyways it is preferable to use a Cam2D instead of a FixedCamera
    */
    class FixedCamera {

    protected:
        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;

    public:
        /**
         * @brief Default constructor, creates an upright camera at the position
         *        (0, 0, -3) pointing to the origin
        */
        FixedCamera();

        /**
         * @brief Creates an upright camera at a given position, pointing to the
         *        origin
         * 
         * @param position The camera's position (in 3D) 
        */
        FixedCamera(glm::vec3 position);

        /**
         * @brief Creates a camera with a given orientaton
         * 
         * @param position The camera's position
         * @param front The point in front of the camera (where the camera will
         *              be pointing at)
         * @param up The camera's up vector
        */
        FixedCamera(glm::vec3 position, glm::vec3 front, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

        /**
         * @brief Sets the camera's position
         * @param position The camera's new position
        */
        void setPosition(glm::vec3 position);

        /**
         * @brief Get the camera's current position
         * @return The camera's position
        */
        glm::vec3 getPosition() const;

        /**
         * @brief Sets the point the camera is pointing to
         * @param front A point
        */
        void setFront(glm::vec3 front);

        /**
         * @brief Get the front of the camera
         * @return The point the camera is pointing to
        */
        glm::vec3 getFront() const;

        /**
         * @brief Sets the camera's up vector
         * @param up The new up vector
        */
        void setUp(glm::vec3 up);

        /**
         * @brief Get the camera's up vector
         * @return The camera's up vector
        */
        glm::vec3 getUp() const;

        /**
         * @brief Returns the camera's "look_at" matrix, it is used internally
         *        by a GLWindow to set its view when drawing things
         * @return The camera's "look_at" matrix
        */
        glm::mat4 lookAt() const;
    };

}


#endif //MY_FIXED_CAMERA