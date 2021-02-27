#ifndef MY_ANIMATION
#define MY_ANIMATION

#include "Drawable/AbstractShape.hpp"
#include <SDL.h>

namespace my
{
    /**
     * @brief A class for animating shapes
    */
    class MYGL_EXPORT Animation 
    {
    private:
        float m_duration;
        glm::vec2 m_originalPos;
        glm::vec2 m_targetPos;
        glm::vec2 m_positionStep;
        float m_frametime;
        AbstractShape* p_shape;
        bool m_running;

    public:

        /**
         * @brief Creates an 'empty' animation whiwh is not usable until a shape Has been
         *        attached to it
        */
        Animation();

        /**
         * @brief Creates an 'empty' animation with a shape attached to it
         * 
         * This contructor only attaches the shape to the animation, the 
         * duration and destination coordinates should then be specified 
         * using the setDuration() and setTargetPosition() functions
         * 
         * @param shape The shape we want to animate
        */
        Animation(AbstractShape& shape);

        /**
         * @brief Creates an animation of a shape moving towards a point
         * @param shape The shape we want to animate
         * @param duration The animation's duration
         * @param destX The x coordinate of the destination
         * @param destY The y coordinate of the destination
        */
        Animation(AbstractShape& shape, float duration, float destX, float destY);

        /**
         * @brief Starts the animation
         * 
         * To avoid the shape suddenly changing position the first time
         * its drawn, this method shouldn't be called too long before the
         * shape is drawn
        */
        void start();

        /**
         * @brief Sets the shape corresponding to this animation
         * 
         * The shape should not be destroyed while the animation is used as
         * it keeps a reference to the shape
         * 
         * @param shape The shape to be animated
        */
        void attach(AbstractShape& shape);

        /**
         * @brief Resets an animation
         *      
         * This method sets the attached shape back to its original position,
         * the animation can then be restarted with a call to start()
        */
        void reset();

        /**
         * @brief Sets the duration of the animation
         * @param duration The desired duration
        */
        void setDuration(float duration);

        /**
         * @brief Sets the shape's target position
         * @param x The target position's x coordinate
         * @param y The target position's y coordinate
         * 
         * This method can be called while the animation is running, it will modify
         * the target position and move the shape towards that position with the original
         * duration
        */
        void setTargetPosition(float x, float y);

        /**
         * @brief Sets the shape's target position
         * @param pos The target position
         * 
         * This method can be called while the animation is running, it will modify
         * the target position and move the shape towards that position with the original
         * duration
        */
        void setTargetPosition(const glm::vec2& pos);

        /**
         * @brief 
         * @param speed 
        */
        void setSpeed(float speed); //TODO

        /**
         * @brief Tells if the animation is over
         * @return true if the animation has ended or has not already begun
        */
        bool isOver() const;

        /**
         * @brief Updates and draws the associated shape, this method is called by
         *        a window
         * @param lookAt The view matrix (usually provided by the window)
         * @param projection The projection matrix (also provided by the window)
        */
        void draw(const glm::mat4& lookAt, const glm::mat4& projection);
    };
}

#endif //MY_ANIMATION