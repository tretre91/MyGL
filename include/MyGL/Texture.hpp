#ifndef MY_TEXTURE
#define MY_TEXTURE

#include <glad/glad.h>
#include <stb_image.h>

#include "Color.hpp"

#include <string>
#include <unordered_map>

namespace my
{
    class MYGL_EXPORT Texture
    {
    private:
        static std::unordered_map<unsigned int, unsigned int> instances;

        unsigned int m_textureId = 0;
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
         * @brief Creates an empty texture whiwh is not usable until an image as been loaded
         */
        Texture() = default;

        /**
         * @brief Creates a texture from an image file
         * @param filename The path to the image
         * @param format Image format, if the source image contains an alpha channel the parameter 'GL_RGBA' should be used
         */
        Texture(const std::string& filename, GLenum format = GL_RGB);

        /**
         * @brief Cretes a texture from an existing opengl texture
         * @param textureId The texture's id, created with glGenTextures
         * @param width The texture's width in pixels
         * @param height The texture's height in pixels
         */
        Texture(unsigned int textureId, unsigned int width, unsigned int height);

        ~Texture();

        Texture(const Texture& texture);

        Texture& operator=(const Texture& texture);

        Texture(Texture&&) noexcept;

        Texture& operator=(Texture&&) noexcept;

        /**
         * @brief Loads a texture, this texture should not be used if the operation fails
         * @param filename The path to the image
         * @param format Image format, if the source image contains an alpha channel the parameter 'GL_RGBA' should be used
         * @return True if the image has been successfuly loaded, false if an error has occured
         */
        bool load(const std::string& filename, GLenum format = GL_RGB);

        /**
         * @brief Return a texture's OpenGL id
         * @return The texture's id
         */
        unsigned int getId() const;

        /**
         * @brief Gives the texture's original width in pixels
         * @return The texture's width
         */
        unsigned int getWidth() const;

        /**
         * @brief Gives the texture's original height in pixels
         * @return The texture's height
         */
        unsigned int getHeight() const;

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
        void setBorderColor(const my::Color& color);
    };

} // namespace my

#endif // MY_TEXTURE