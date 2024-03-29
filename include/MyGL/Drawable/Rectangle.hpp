#ifndef MYGL_RECTANGLE
#define MYGL_RECTANGLE

#include "../mygl_export.h"

#include "AbstractShape.hpp"

#include <array>

namespace my
{
    /**
     * @brief Class for creating rectangles
     */
    class Rectangle : public AbstractShape
    {
    protected:
        static unsigned int VAO;
        static unsigned int VBO;
        static unsigned int EBO;
        static const std::array<float, 20> vertices;
        static const std::array<unsigned int, 4> indices;

        /**
         * @brief Initializes the OpenGL buffer objects
         */
        static void glInit();

        /**
         * @brief Gives a list of the rectangle's points
         * @return A std::vector containing every point (represented by a glm::vec2) in clockwise order
         */
        virtual std::vector<glm::vec2> points() const override;

    public:
        /**
         * @brief Default constructor for the Rectangle class, constructs a grey 10*10 square
         */
        MYGL_EXPORT Rectangle();

        /**
         * @brief Constructs a width * height rectangle
         * @param width The rectangle's width
         * @param height The rectangle's height
         */
        MYGL_EXPORT Rectangle(int width, int height);

        /**
         * @brief Constructs a width * heigth rectangle whose center's coordinate is (x,y)
         * @param width The rectangle's width
         * @param height The rectangle's heigth
         * @param x The x coordinate of the rectangle's center
         * @param y The y coordinate of the rectangle's center
         */
        MYGL_EXPORT Rectangle(int width, int height, int x, int y);

        /** @brief Default destructor */
        MYGL_EXPORT ~Rectangle() override;

        /**
         * @brief Draws a rectangle, this method is called by a window
         * @param lookAt The view matrix (usually provided by the window)
         * @param projection The projection matrix (also provided by the window)
         */
        MYGL_EXPORT virtual void draw(const glm::mat4& lookAt, const glm::mat4& projection) const override;
    };

    /** @relates my::Rectangle
     * @fn my::Rectangle my::line(int x1, int y1, int x2, int y2)
     * @brief Creates a line between two points
     * @param x1 The first point's x coordinate
     * @param y1 The first point's y coordinate
     * @param x2 The second point's x coordinate
     * @param y2 The second point's y coordinate
     * @return A my::Rectangle representing a line between the 2 points
     */
    MYGL_EXPORT Rectangle line(int x1, int y1, int x2, int y2);

} // namespace my

#endif // MYGL_RECTANGLE
