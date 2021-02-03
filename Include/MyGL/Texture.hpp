#ifndef MY_TEXTURE
#define MY_TEXTURE

#include "Header.hpp"

#include <string>
#include <iostream>

namespace my
{

    class MYGL_EXPORT Texture {

    private:
        unsigned int id;

    public:
        
        enum class Axis { x, y, s, t };

        /**
         * @brief Creates a texture from an image file
         * @param filename The path to the image
         * @param format Image format, if the source image contains an alpha channel the parameter 'GL_RGBA' should be used
        */
        Texture(const std::string& filename, GLenum format = GL_RGB);

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
    };

}

#endif // MY_TEXTURE