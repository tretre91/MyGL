#ifndef MY_3D_CAMERA
#define MY_3D_CAMERA

#include "MovableCamera.hpp"

namespace my
{
    
    class Cam3D : public MovableCamera {

    protected:
        float m_sensitivity;
        float m_pitch;
        float m_yaw;

    public:
        Cam3D();
        Cam3D(glm::vec3 position);
        Cam3D(glm::vec3 position, glm::vec3 front, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

        void moveUp(float frametime);
        void moveDown(float frametime);
        virtual void moveFront(float frametime);
        virtual void moveBack(float frametime);
        void moveLeft(float frametime);
        void moveRight(float frametime);
        void moveView(float pitch, float yaw);

        void setSensitivity(float sensi);
        float getSensitivity() const;
    };

}

#endif //MY_3D_CAMERA