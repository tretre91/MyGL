#ifndef MYGL_TEXTURE
#define MYGL_TEXTURE

#include <glad/glad.h>
#include <stb_image.h>

#include "Color.hpp"

#include <memory>
#include <string>

namespace my
{
    /**
     * @brief Class for storing textures
     */
    class MYGL_EXPORT Texture
    {
    private:
        std::shared_ptr<unsigned int> p_textureId;
        unsigned int m_width = 0;
        unsigned int m_height = 0;

    public:
        enum class Axis
        {
            x,
            y,
            s,
            t
        };

        /**
         * @brief Creates an empty texture whiwh is not usable until an image has been loaded
         */
        Texture() noexcept = default;

        /**
         * @brief Creates a texture from an image file
         * @param filename The path to the image
         */
        Texture(const std::string& filename);

        /**
         * @brief Cretes a texture from an existing opengl texture
         * @param textureId The texture's id, created with glGenTextures
         * @param width The texture's width in pixels
         * @param height The texture's height in pixels
         */
        Texture(unsigned int textureId, unsigned int width, unsigned int height);

        ~Texture() = default;

        /**
         * @brief Loads a texture, this texture should not be used if the operation fails
         * @param filename The path to the image
         * @return True if the image has been successfuly loaded, false if an error has occured
         */
        bool load(const std::string& filename);

        /**
         * @brief Return a texture's OpenGL id
         * @return The texture's id
         */
        unsigned int getId() const noexcept;

        /**
         * @brief Gives the texture's original width in pixels
         * @return The texture's width
         */
        unsigned int getWidth() const noexcept;

        /**
         * @brief Gives the texture's original height in pixels
         * @return The texture's height
         */
        unsigned int getHeight() const noexcept;

        /**
         * @brief Binds this texture to GL_TEXTURE_2D
         */
        void bind() const;

        /**
         * @brief Sets the wrapping method for a given axis
         * @param axis the axis (s/x or t/y) we want to apply the wrapping method to
         * @param method The wrapping method to be applied (valid methods are GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE and GL_CLAMP_TO_BORDER)
         */
        void setTextureWrapMethod(Axis axis, GLenum method);

        /**
         * @brief Sets the border color used with GL_CLAMP_TO_BORDER wrapping method
         * @param r The red component
         * @param g The green component
         * @param b The blue component
         * @param alpha The alpha component
         */
        void setBorderColor(int r, int g, int b, int alpha = 255);

        /**
         * @brief Sets the border color used with GL_CLAMP_TO_BORDER wrapping method
         * @param color The color to be applied
         */
        void setBorderColor(const Color& color);
    };

} // namespace my

#endif // MYGL_TEXTURE