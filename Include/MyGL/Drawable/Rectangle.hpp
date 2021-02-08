#ifndef MY_RECTANGLE
#define MY_RECTANGLE

#include "../Shader.hpp"
#include "AbstractShape.hpp"

namespace my
{
    /**
     * @brief Class for drawing rectangles
    */
    class MYGL_EXPORT Rectangle : public AbstractShape {
    protected:
        static unsigned int VAO;
        static unsigned int VBO;
        static unsigned int EBO;
        static const std::array<float, 20> vertices;
        static const std::array<unsigned int, 4> indices;
        bool moveByCenter;
        virtual void glInit();

    public:

        /**
         * @brief Default constructor for the Rectangle class, constructs a grey 10*10 square
        */
        Rectangle();

        /**
         * @brief Constructs a width * height rectangle
         * @param width The rectangle's width
         * @param height The rectangle's height
        */
        Rectangle(int width, int height);

        /**
         * @brief Constructs a width * heigth rectangle whose center's coordinate is (x,y)
         * @param width The rectangle's width
         * @param height The rectangle's heigth
         * @param x The x coordinate of the rectangle's center
         * @param y The y coordinate of the rectangle's center
        */
        Rectangle(int width, int height, int x, int y);

        /**
         * @brief Allows to modify the anchor point used to specify the rectangle's position (which is by default the top left hand corner)
         * @param center If center is true then setPosition will set the rectangle's center at the specified position, otherwise the top left hand corner will be placed at that position
        */
        void positionByCenter(bool center);

        /**
         * @brief Gives a list of the rectangle's points
         * @return A std::vector containing every point (represented by a glm::vec2) in clockwise order
        */
        virtual std::vector<glm::vec2> points() const;

        // /** //TODO
        //  * 
        //  * 
        //  * 
        // */
        // virtual void setTexture(const std::string& filename, bool hasAlhpa = false);

        /**
         * @brief Draws a rectangle, this method is called by a window
         * @param lookAt The view matrix (usually provided by the window)
         * @param projection The projection matrix (also provided by the window)
        */
        virtual void draw(const glm::mat4& lookAt, const glm::mat4& projection);
    };
}

#endif //MY_RECTANGLE