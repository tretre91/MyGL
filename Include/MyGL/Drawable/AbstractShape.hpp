#ifndef MY_ABSTRACT_SHAPE
#define MY_ABSTRACT_SHAPE

#include "../Shader.hpp"
#include "../Camera/FixedCamera.hpp"
#include "../Vector.hpp"
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

        glm::vec2 position;
        const glm::vec2 originalScale;
        glm::vec2 scaleFactor;
        int rotationAngle;
        bool updateMatrix;
        my::Color color;
        glm::mat4 model;
        my::Texture texture;
        my::Shader* activeShader = nullptr;

        
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
         * @brief Moves the object relative to it's actual position
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
        void setRotation(int angle);

        /**
         * @brief Rotates the object
         * @param angle The angle (in degrees) to add to the current angle of rotation
        */
        void rotate(int angle);

        /**
         * @brief Gives the object's angle of rotation
         * @return The angle of the rotation currently applied to the object
        */
        int getRotation() const;

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
         * @brief Tells wether 2 shapes are overlapping
         * @param shape The shape to test against
         * @return true if the this shape is overlapping with the specified shape
        */
        bool colides(AbstractShape* shape) const;

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