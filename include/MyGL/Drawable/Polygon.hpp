#ifndef MYGL_POLYGON
#define MYGL_POLYGON

#include "../mygl_export.h"

#include "AbstractShape.hpp"

#include <unordered_map>
#include <vector>

namespace my
{
    /**
     * @brief Class for creating regular polygons of 3 or more sides
     */
    class Polygon : public AbstractShape
    {
    private:
        struct GLinfo {
            unsigned int vao;
            unsigned int vbo;
            unsigned int ebo;
            std::vector<float> vertices;
            std::vector<unsigned int> indices;
        };

        static std::unordered_map<unsigned int, const GLinfo> buffers;
        unsigned int m_sides;
        const GLinfo* p_buffer;

        /**
         * @brief Compute a polygon's vertices' normalized positions and stores them in a GLinfo object
         * @param sides The number of sides of the polygon
         * @param buffer A GLinfo in which the resulting vertices and indices will be stored
         */
        static void computeVertices(unsigned int sides, GLinfo& buffer);

        /**
         * @brief Initializes the OpenGL buffer objects for a polygon
         * @param sides The polygon's number of sides
         */
        static void glInit(unsigned int sides);

        /**
         * @brief Gives a list of the polygon's points
         * @return A std::vector containing every point (represented by a glm::vec2)
         *         in counter-clockwise order
         */
        virtual std::vector<glm::vec2> points() const override;

    public:
        /**
         * @brief Creates a triangle of radius 10
         */
        MYGL_EXPORT Polygon();

        /**
         * @brief Creates a n-sided regular polygon with the specified radius
         * @param sides The number of sides (at least 3)
         * @param radius The polygon's radius
         */
        MYGL_EXPORT Polygon(unsigned int sides, int radius);

        /**
         * @brief Creates a n-sided regular polygon with the specified radius
         *        and places its center at (x, y)
         * @param sides The number of sides (at least 3)
         * @param radius The polygon's radius
         * @param x The x coordinate of the center
         * @param y The y coordinate of the center
         */
        MYGL_EXPORT Polygon(unsigned int sides, int radius, int x, int y);

        /**
         * @brief Changes the number of sides of a polygon
         * @param sides The new number of sides
         */
        MYGL_EXPORT void setSides(unsigned int sides);

        /**
         * @brief Draws a polygon, this method is called by a window
         * @param lookAt The view matrix (usually provided by the window)
         * @param projection The projection matrix (also provided by the window)
         */
        MYGL_EXPORT virtual void draw(const glm::mat4& lookAt, const glm::mat4& projection) const override;
    };

} // namespace my

#endif // MYGL_POLYGON