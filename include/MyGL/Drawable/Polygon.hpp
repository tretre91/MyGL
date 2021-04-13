#ifndef MY_POLYGON
#define MY_POLYGON

#include "../mygl_export.h"
#include "AbstractShape.hpp"

#include <vector>
#include <unordered_map>

namespace my
{
    /**
     * @brief Class for creating regular polygons of 3 or more sides
    */
    class MYGL_EXPORT Polygon : public AbstractShape {
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

        static void computeVertices(unsigned int sides, GLinfo& buffer);

        static void glInit(unsigned int sides);

        /**
         * @brief Gives a list of the polygon's points
         * @return A std::vector containing every point (represented by a glm::vec2) in counter-clockwise order
        */
        virtual std::vector<glm::vec2> points() const;

    public:

        /**
         * @brief Creates a triangle of radius 10
        */
        Polygon();
        
        /**
         * @brief Creates a n-sided regular polygon with the specified radius
         * @param sides The number of sides (at least 3)
         * @param radius The polygon's radius
        */
        Polygon(unsigned int sides, int radius);

        /**
         * @brief Creates a n-sided regular polygon with the specified radius
         *        and places its center at (x, y)
         * @param sides The number of sides (at least 3)
         * @param radius The polygon's radius
         * @param x The x coordinate of the center
         * @param y The y coordinate of the center
        */
        Polygon(unsigned int sides, int radius, int x, int y);

        /**
         * @brief Changes the number of sides of a polygon
         * @param sides The new number of sides
        */
        void setSides(unsigned int sides);

        /**
         * @brief Draws a polygon, this method is called by a window
         * @param lookAt The view matrix (usually provided by the window)
         * @param projection The projection matrix (also provided by the window)
        */
        virtual void draw(const glm::mat4& lookAt, const glm::mat4& projection);
    };

} //namespace my

#endif //MY_POLYGON