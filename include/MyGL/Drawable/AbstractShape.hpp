#ifndef MYGL_ABSTRACT_SHAPE
#define MYGL_ABSTRACT_SHAPE

#include "../mygl_export.h"

#include "../Camera.hpp"
#include "../Color.hpp"
#include "../ShaderProgram.hpp"
#include "../Texture.hpp"

#include <string>
#include <vector>

namespace my
{
    /**
     * @brief Class inherited by all the shapes, it defines common operations on a shape
     */
    class MYGL_EXPORT AbstractShape
    {
    private:
        static void initShaders();

    protected:
        static const float pi;
        static my::ShaderProgram shader;
        static my::ShaderProgram texShader;

        glm::vec2 m_position;
        glm::vec2 m_originalScale;
        glm::vec2 m_scaleFactor;
        float m_rotationAngle;
        mutable bool m_updateMatrix;
        mutable glm::mat4 m_model;
        my::Color m_color;
        float m_outlineThickness;
        my::Color m_outlineColor;
        mutable glm::mat4 m_outlineModel;
        my::Texture m_texture;
        bool m_isTextured;
        my::ShaderProgram m_shader;
        my::ShaderProgram m_outlineShader;

        /**
         * @brief Returns the points composing this shape
         * @return The coordinates of each of the shape's points
         */
        virtual std::vector<glm::vec2> points() const = 0;

    public:
        static const ShaderProgram& defaultShader;

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
         * @brief Returns the size of the shape
         * @return The size of the shape's bounding box
         */
        glm::vec2 getSize() const noexcept;

        /**
         * @brief Moves the shape to (x,y)
         * @param x The new horizontal position
         * @param y The new vertical popsition
         * @param center If true, set the shape's center at (x,y), else sets its
         *               bottom left hand corner at (x,y). Default value is false
         */
        virtual void setPosition(int x, int y, bool center = false);

        /**
         * @brief Moves the shape to a specified position
         * @param pos A glm::vec2 containing the new position
         * @param center If true, set the shape's center at 'pos', else sets its
         *               bottom left hand corner. Default value is false
         */
        virtual void setPosition(const glm::vec2& pos, bool center = false);

        /**
         * @brief Moves the shape to a certain position, relative to a camera's position
         * @param x The new horizontal position in the camera's view
         * @param y The vertical position in the camera's view
         * @param camera A my::Camera
         * @param center If true, set the shape's center at (x,y), else sets its
         *               bottom left hand corner at (x,y). Default value is false
         */
        virtual void setRelativePosition(int x, int y, const my::Camera& camera, bool center = false);

        /**
         * @brief Moves the shape to a certain position, relative to a camera's position
         * @param pos A glm::vec2 containing the new position
         * @param camera A my::Camera
         * @param center If true, set the shape's center at (x,y), else sets its
         *               bottom left hand corner at (x,y). Default value is false
         */
        virtual void setRelativePosition(const glm::vec2& pos, const my::Camera& camera, bool center = false);

        /**
         * @brief Moves the shape relative to it's current position
         * @param x The horizontal offset
         * @param y The vertical offset
         */
        virtual void move(float x, float y) noexcept;

        /**
         * @brief Gives the current position of the shape's center
         * @return The position of the object's center
         */
        glm::vec2 getPosition() const noexcept;

        /**
         * @brief Sets the scale factor applied to the shape
         * @param x The horizontal scaling factor
         * @param y The vertical scaling factor
         */
        void setScale(float x, float y) noexcept;

        /**
         * @brief Modifies the scaling factor
         * @param x The value to add to the horizontal scaling factor
         * @param y The value to add to the vertical scaling factor
         */
        void scale(float x, float y) noexcept;

        /**
         * @brief Gives the current scaling factor applied to the shape
         * @return The scaling factor currently applied
         */
        glm::vec2 getScale() const noexcept;

        /**
         * @brief Sets the rotation applied to the shape
         * @param angle the angle (in degrees) of rotation
         */
        void setRotation(float angle);

        /**
         * @brief Rotates the shape
         * @param angle The angle (in degrees) to add to the current angle of rotation
         */
        void rotate(float angle);

        /**
         * @brief Returns the shapes's angle of rotation
         * @return The angle of the rotation currently applied to the object
         */
        float getRotation() const noexcept;

        /**
         * @brief Sets the shape's color from its individual components (ints between 0 and 255)
         * @param r The red component
         * @param g The green component
         * @param b The blue component
         * @param alpha The alpha (transparency) value, by default 255
         */
        virtual void setColor(int r, int g, int b, int alpha = 255) noexcept;

        /**
         * @brief Sets the shape's color from an existing color
         * @param color The color to be applied to the shape
         */
        virtual void setColor(const my::Color& color) noexcept;

        /**
         * @brief Returns the shape's color
         * @return A my::Color object containing the shape's color
         */
        my::Color getColor() const noexcept;

        /**
         * @brief Sets the shape's outline's thickness
         * @param thickness The thickness in pixels
         */
        void setOutlineThickness(unsigned int thickness);

        /**
         * @brief Sets the shape's outline color
         * @param color The color to be applied
         */
        void setOutlineColor(const my::Color& color) noexcept;

        /**
         * @brief Sets the shape's outline color from its individual components (ints between 0 and 255)
         * @param r The red component
         * @param g The green component
         * @param b The blue component
         * @param alpha The alpha (transparency) value, by default 255
         */
        void setOutlineColor(int r, int g, int b, int alpha = 255) noexcept;

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
         * This method is less expensive than the SAT one but far less precise (and it
         * doesn't work properly when the shapes are rotated). If you need more precision
         * use the SATCollides method instead.
         */
        bool BBoxCollides(const AbstractShape& shape) const noexcept;

        /**
         * @brief Attaches a texture to the shape
         * @param texture A texture that has already been initialized
         */
        virtual void setTexture(const my::Texture& texture);

        /**
         * @brief Attaches a texture to the shape
         * @param filename The path to the image, supported formats are jpeg, png,
         *                 gif (not animated), bmp, tga, psd, hdr, pic, and pnm
         */
        void setTexture(const std::string& filename);

        /**
         * @brief Sets the shader used to draw the shape
         * @param program The shader program to use
         */
        void setShader(const ShaderProgram& program);

        /**
         * @brief Sets the shader used to draw the shape's outline
         * @param program The shader program to use
         */
        void setOutlineShader(const ShaderProgram& program);

        /**
         * @brief Draws a shape, this method is called by a window
         * @param lookAt The view matrix (usually provided by the window)
         * @param projection The projection matrix (also provided by the window)
         */
        virtual void draw(const glm::mat4& lookAt, const glm::mat4& projection) const = 0;
    };

} // namespace my

#endif // MYGL_ABSTRACT_SHAPE
