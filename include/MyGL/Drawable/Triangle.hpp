#ifndef MY_TRIANGLE
#define MY_TRIANGLE

#include "../mygl_export.h"

#include "AbstractShape.hpp"

#include <array>

namespace my
{
    /**
     * @brief Class for drawing triangles
     */
    class Triangle : public AbstractShape
    {
    private:
        unsigned int m_vao;
        unsigned int m_vbo;
        std::array<float, 15> m_vertices;

        /**
         * @brief Gives a list of the triangle's points
         * @return A std::vector containing every point (represented by a glm::vec2)
         *         in counter-clockwise order
         */
        std::vector<glm::vec2> points() const override;

    public:
        /**
         * @brief Creates a triangle from its point's coordinates
         * @param x1, y1 The first point's coordinates
         * @param x2, y2 The second point's coordinates
         * @param x3, y3 The third point's coordinates
         */
        Triangle(float x1, float y1, float x2, float y2, float x3, float y3);

        /**
         * @brief Creates a triangle from 3 points
         * @param p1, p2, p3 The triangle's points
         */
        Triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);

        /**
         * @brief Draws the triangle, this method is called by a window
         * @param lookAt The view matrix (usually provided by the window)
         * @param projection The projection matrix (also provided by the window)
         */
        void draw(const glm::mat4& lookAt, const glm::mat4& projection) override;
    };
} // namespace my

#endif // MY_TRIANGLE