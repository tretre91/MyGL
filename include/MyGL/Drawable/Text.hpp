#ifndef MY_CONST_TEXT
#define MY_CONST_TEXT

#include "../mygl_export.h"
#include "Rectangle.hpp"
#include "Font.hpp"

#include <locale>
#include <codecvt>

namespace my
{
    class MYGL_EXPORT Text : public Rectangle {
    private:
        static const std::string textVertexSource;
        static const std::string textFragmentSource;
        static my::Shader textShader;
        static my::Font defaultFont;
        static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> u8_u32conv;
        static std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> u16_u32conv;
        static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> u8_wconv;

        std::u32string m_text;
        my::Font& m_font;
        unsigned int m_size;

        void setTexture(const my::Texture& texture);

    public:
        Text();
        Text(const std::string& text, my::Font& font, unsigned int size = 30u);
        Text(const std::wstring& text, my::Font& font, unsigned int size = 30u);
        Text(const std::u16string& text, my::Font& font, unsigned int size = 30u);
        Text(const std::u32string& text, my::Font& font, unsigned int size = 30u);

        void setContent(const std::string& text);
        void setContent(const std::wstring& text);
        void setContent(const std::u16string& text);
        void setContent(const std::u32string& text);

        std::string getString() const;
        std::wstring getWString() const;
        std::u16string getU16String() const;
        std::u32string getU32String() const;

        void setFont(my::Font& font);

        void setFontSize(unsigned int size);
    };
}

#endif //MY_CONST_TEXT