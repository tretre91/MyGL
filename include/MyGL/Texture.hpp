#ifndef MYGL_TEXTURE
#define MYGL_TEXTURE

#include "mygl_export.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "Color.hpp"
#include "Image.hpp"

#include <memory>
#include <string>

namespace my
{
    /**
     * @brief Class for storing textures
     */
    class Texture
    {
    private:
        std::shared_ptr<unsigned int> p_textureId;
        unsigned int m_width = 0;
        unsigned int m_height = 0;

        void create(const Image& image);

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
        MYGL_EXPORT Texture() noexcept = default;

        /**
         * @brief Creates a texture from an image file
         * @param filename The path to the image
         */
        MYGL_EXPORT Texture(const std::string& filename);

        /**
         * @brief Creates a texture fron an image
         * @param image An Image object
         */
        MYGL_EXPORT Texture(const Image& image);

        /**
         * @brief Cretes a texture from an existing opengl texture
         * @param textureId The texture's id, created with glGenTextures
         * @param width The texture's width in pixels
         * @param height The texture's height in pixels
         */
        MYGL_EXPORT Texture(unsigned int textureId, unsigned int width, unsigned int height);

        MYGL_EXPORT ~Texture() = default;

        /**
         * @brief Loads a texture, this texture should not be used if the operation fails
         * @param filename The path to the image
         * @return True if the image has been successfuly loaded, false if an error has occured
         */
        MYGL_EXPORT bool load(const std::string& filename);

        /**
         * @brief Creates a texture from an Image
         * @param image An Image object
         * @return True if the texture has been successfuly created, false if an error has occured
         */
        MYGL_EXPORT bool load(const Image& image);

        /**
         * @brief Return a texture's OpenGL id
         * @return The texture's id
         */
        MYGL_EXPORT unsigned int getId() const noexcept;

        /**
         * @brief Gives the texture's original width in pixels
         * @return The texture's width
         */
        MYGL_EXPORT unsigned int getWidth() const noexcept;

        /**
         * @brief Gives the texture's original height in pixels
         * @return The texture's height
         */
        MYGL_EXPORT unsigned int getHeight() const noexcept;

        /**
         * @brief Binds this texture to GL_TEXTURE_2D
         */
        MYGL_EXPORT void bind() const;

        /**
         * @brief Sets the wrapping method for a given axis
         * @param axis the axis (s/x or t/y) we want to apply the wrapping method to
         * @param method The wrapping method to be applied (valid methods are GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE and GL_CLAMP_TO_BORDER)
         */
        MYGL_EXPORT void setTextureWrapMethod(Axis axis, GLenum method);

        /**
         * @brief Sets the border color used with GL_CLAMP_TO_BORDER wrapping method
         * @param r The red component
         * @param g The green component
         * @param b The blue component
         * @param alpha The alpha component
         */
        MYGL_EXPORT void setBorderColor(int r, int g, int b, int alpha = 255);

        /**
         * @brief Sets the border color used with GL_CLAMP_TO_BORDER wrapping method
         * @param color The color to be applied
         */
        MYGL_EXPORT void setBorderColor(const Color& color);
    };

} // namespace my

#endif // MYGL_TEXTURE