#ifndef MY_TEXT
#define MY_TEXT

#include "../mygl_export.h"
#include "AbstractShape.hpp"
#include "Font.hpp"

namespace my
{
    /**
     * @brief Class for creating strings which can be rendered on the screen
    */
    class MYGL_EXPORT Text : public AbstractShape {
    private:
        /**
         * @brief The vertex shader source code used by this classe's shader
        */
        static const std::string textVertexSource;

        /**
         * @brief The fragment shader source code used by this classe's shader
        */
        static const std::string textFragmentSource;

        /**
         * @brief The shader shared by all the instances of this class
        */
        static my::Shader textShader;

        std::string m_text;
        unsigned int m_size;
        unsigned int m_fontScale;
        unsigned int m_bitmapTexId;
        std::vector<std::pair<int, int>> m_charPositions;
        std::vector<my::Font::Glyph>& m_alphabet;

    public:
        /**
         * @brief Creates a string
         * @param text The string's content
         * @param font The font used to create this string
         * @param size The character size (in pixels)
        */
        Text(const std::string& text, my::Font& font, unsigned int size = 30u);

        /**
         * @brief Indicates the points composing this shape
         * @return The coordinates of the points of this shape
        */
        virtual std::vector<glm::vec2> points() const;

        /**
         * @brief Draws a string, this method is called by a window
         * @param lookAt The view matrix (usually provided by the window)
         * @param projection The projection matrix (also provided by the window)
        */
        virtual void draw(const glm::mat4& lookAt, const glm::mat4& projection);
    };
}

#endif //MY_TEXT