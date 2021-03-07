#ifndef MY_CONST_TEXT
#define MY_CONST_TEXT

#include "../mygl_export.h"
#include "Rectangle.hpp"
#include "Font.hpp"

namespace my
{
    class MYGL_EXPORT Text : public Rectangle {
    private:
        static const std::string textVertexSource;
        static const std::string textFragmentSource;
        static my::Shader textShader;

        std::string m_text;
        my::Font& m_font;
        unsigned int m_size;

        void setTexture(const my::Texture& texture);

    public:
        Text(const std::string& text, my::Font& font, unsigned int size = 30u);

        void setContent(const std::string& text);

        std::string getContent() const;

        void setFont(my::Font& font);

        void setFontSize(unsigned int size);
    };
}

#endif //MY_CONST_TEXT