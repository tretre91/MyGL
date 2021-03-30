#ifndef MY_TEXT
#define MY_TEXT

#include "../mygl_export.h"
#include "Rectangle.hpp"
#include "Font.hpp"

#include <locale>
#include <codecvt>

namespace my
{
    /**
     * @brief Class for drawing strings of text
    */
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
        /**
         * @brief Default constructor, creates an empty string.
         * 
         * This constructor initializes the text to an empty string, the character size to 0
         * and uses a placeholder font. Before drawing this object you should set a usable font
         * with the setFont() function
        */
        Text();

        /** @name Constructors
         * @brief Constructors for the text class
         * @param text A std::string (or wstring, u16string, u32string) with the text to
         *        be displayed
         * @param font A my::Font object defining the text's font
         * @param size The character size in pixels
        */
        /** @{ */
        Text(const std::string& text, my::Font& font, unsigned int size = 30u);
        Text(const std::wstring& text, my::Font& font, unsigned int size = 30u);
        Text(const std::u16string& text, my::Font& font, unsigned int size = 30u);
        Text(const std::u32string& text, my::Font& font, unsigned int size = 30u);
        /** @} */

        /** @name Setters
         * @brief Sets the content of the displayed text
         * @param text The new string to display
        */
        /** @{ */
        void setContent(const std::string& text);
        void setContent(const std::wstring& text);
        void setContent(const std::u16string& text);
        void setContent(const std::u32string& text);
        /** @} */

        /** @name Getters
         * @brief Returns the text that is currently displayed
         * @return The content of the string that is currently displayed
        */
        /** @{ */
        std::string getString() const;
        std::wstring getWString() const;
        std::u16string getU16String() const;
        std::u32string getU32String() const;
        /** @} */

        /**
         * @brief Changes the font
         * @param font The new font
        */
        void setFont(my::Font& font);

        /**
         * @brief Changes the character size
         * @param size The new character size (in pixels)
        */
        void setFontSize(unsigned int size);
    };
}

#endif //MY_TEXT