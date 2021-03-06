#ifndef MY_CONST_TEXT
#define MY_CONST_TEXT

#include "../mygl_export.h"
#include "Rectangle.hpp"
#include "Font.hpp"

namespace my
{
    class MYGL_EXPORT ConstText : public Rectangle {
    private:
        static const std::string textVertexSource;
        static const std::string textFragmentSource;
        static my::Shader textShader;

    public:
        ConstText(const std::string& text, my::Font& font, unsigned int size = 30u);
    };
}

#endif //MY_CONST_TEXT