#ifndef MY_ABSTRACT_SHAPE
#define MY_ABSTRACT_SHAPE

#include "../mygl_export.h"
#include "../Shader.hpp"
#include "../Color.hpp"
#include "../Texture.hpp"

#include <vector>
#include <algorithm>

namespace my
{
    class MYGL_EXPORT AbstractShape {
    protected:
        static const float pi;
        
        static const std::string vertexSource;
        static const std::string fragmentSource;
        static my::Shader shader;

        static const std::string texVertexSource;
        static const std::string texFragmentSource;
        static my::Shader texShader;
        static bool shaderIsUsable;

        glm::vec2 m_position;
        glm::vec2 m_originalScale;
        glm::vec2 m_scaleFactor;
        float m_rotationAngle;
        bool m_updateMatrix;
        glm::mat4 m_model;
        my::Color m_color;
        unsigned int m_outlineThickness;
        my::Color m_outlineColor;
        glm::mat4 m_outlineModel;
        my::Texture m_texture;
        bool m_isTextured;
        my::Shader* p_activeShader;

        
        /**
         * @brief Indicates the points composing this shape
         * @return The coordinates of the points of this shape
        */
        virtual std::vector<glm::vec2> points() const = 0;

    public:
        /**
         * @brief Default constructor, Initializes a 10 * 10 grey object
        */
        AbstractShape();

        /**
         * @brief Intializes a grey object which dimensions are width * height
         * @param width The object's width
         * @param height The object's height
        */
        AbstractShape(int width, int height);

        /**
         * @brief Initializes a width * height object whose center is at (x, y);
         * @param width The object's width
         * @param height The object's height
         * @param x x coordinate of the center
         * @param y y coordinate of the center
        */
        AbstractShape(int width, int height, int x, int y);

        /**
         * @brief Moves the shape to (x,y)
         * @param x The new horizontal position
         * @param y The new vertical popsition
         * @param center If true, set the shape's center at (x,y), else sets its
         *               top left hand corner at (x,y). Default value is false
        */
        virtual void setPosition(int x, int y, bool center = false);

        /**
         * @brief Moves to shape to a specified position
         * @param pos A glm::vec2 containing the new position
         * @param center If true, set the shape's center at 'pos', else sets its
         *               top left hand corner. Default value is false
        */
        virtual void setPosition(const glm::vec2& pos, bool center = false);

        /**
         * @brief Moves the object relative to it's current position
         * @param x The horizontal offset
         * @param y The vertical offset
        */
        virtual void move(float x, float y);

        /**
         * @brief Gives the actual position of the object's center
         * @return The position of the object's center
        */
        glm::vec2 getPosition() const;

        /**
         * @brief Sets the scale factor applied to the object
         * @param x The horizontal scaling factor
         * @param y The vertical scaling factor
        */
        void setScale(float x, float y);

        /**
         * @brief Changes the actual scaling factor
         * @param x The value to add to the horizontal scaling factor
         * @param y The value to add to the vertical scaling factor
        */
        void scale(float x, float y);

        /**
         * @brief Gives the actual scaling factor applied to the Object
         * @return The scaling factor currently applied
        */
        glm::vec2 getScale() const;

        /**
         * @brief Sets the rotation applied to the Object
         * @param angle the angle (in degrees) of rotation
        */
        void setRotation(float angle);

        /**
         * @brief Rotates the object
         * @param angle The angle (in degrees) to add to the current angle of rotation
        */
        void rotate(float angle);

        /**
         * @brief Gives the object's angle of rotation
         * @return The angle of the rotation currently applied to the object
        */
        float getRotation() const;

        /**
         * @brief Sets the object's color from its individual components (ints between 0 and 255)
         * @param r The red component
         * @param g The green component
         * @param b The blue component
         * @param alpha The alpha (transparency) value, by default 255
        */
        virtual void setColor(int r, int g, int b, int alpha = 255);

        /**
         * @brief Sets the object's color from an existing color
         * @param color The color to be applied to the object
        */
        virtual void setColor(const my::Color& color);

        /**
         * @brief Gives the object's color
         * @return A my::Color object containing the object's color
        */
        my::Color getColor() const;

        /**
         * @brief Sets the object's outline's thickness
         * @param thickness The thickness in pixels
        */
        void setOutlineThickness(unsigned int thickness);

        /**
         * @brief Sets the object's outline color
         * @param color The color to be applied
        */
        void setOutlineColor(const my::Color& color);

        /**
         * @brief Sets the object's outline color from its individual components (ints between 0 and 255)
         * @param r The red component
         * @param g The green component
         * @param b The blue component
         * @param alpha The alpha (transparency) value, by default 255
        */
        void setOutlineColor(int r, int g, int b, int alpha = 255);

        /**
         * @brief Tells wether 2 shapes are overlapping using the separating
         *        axis theorem (SAT)
         * @param shape The shape to test against
         * @return true if the this shape is overlapping with the specified shape
        */
        bool SATCollides(const AbstractShape& shape) const;

        /**
         * @brief Tells whether 2 shapes are overlapping using their bounding 
         *        boxes. 
         * @param shape The shape to test against
         * @return true if the this shape's bounding rectangle is overlapping with the
         *         specified shape's bounding rectangle
         * 
         * This method is less expensive than the SAT one but far less precise, if you
         * need more precision, use the SATCollides method
        */
        bool BBoxCollides(const AbstractShape& shape) const;

        /**
         * @brief Attaches a texture to the shape
         * @param texture A texture that has already been initialized
        */
        virtual void setTexture(const my::Texture& texture);

        /**
         * @brief Attach a texture to the shape
         * @param filename The path to the image, supported formats are jpeg, png,
         *                 gif (not animated), bmp, tga, psd, hdr, pic, and pnm
         * @param hasAlpha True if the image has an alpha channel
        */
        void setTexture(const std::string& filename, bool hasAlpha = false);

        /**
         * @brief Draws a shape, this method is called by a window
         * @param lookAt The view matrix (usually provided by the window)
         * @param projection The projection matrix (also provided by the window)
        */
        virtual void draw(const glm::mat4& lookAt, const glm::mat4& projection) = 0;
    };
}

#endif //MY_ABSTRACT_SHAPE