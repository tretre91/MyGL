#ifndef MYGL_TEXT
#define MYGL_TEXT

#include "../mygl_export.h"

#include "Font.hpp"
#include "Rectangle.hpp"
#include <iterator>
#include <string_view>
#include <utf8.h>

namespace my
{
    namespace util
    {
        template<typename StringType>
        inline std::string toUtf8(const StringType& text) {
            std::basic_string_view view{std::begin(text), std::end(text)};

            using CharType = typename decltype(view)::value_type;

            if constexpr (sizeof(CharType) == 1) {
                return view;
            } else if constexpr (sizeof(CharType) == 2) {
                std::string result;
                result.reserve(2 * view.size());
                utf8::utf16to8(view.begin(), view.end(), std::back_inserter(result));
                return result;
            } else if constexpr (sizeof(CharType) == 4) {
                std::string result;
                result.reserve(4 * view.size());
                utf8::utf32to8(view.begin(), view.end(), std::back_inserter(result));
                return result;
            } else {
                static_assert(false, "Unknown character encoding");
            }
        }
    } // namespace util

    /**
     * @brief Class for drawing strings of text
     */
    class Text : public Rectangle
    {
    private:
        static my::ShaderProgram textShader;
        static my::Font defaultFont;

        std::string m_text;
        my::Font& m_font;
        unsigned int m_size;

        /**
         * @brief Function containing code used in all constructors
         */
        void init();

        /**
         * @brief Override of AbstractShape's setTexture() function
         * @param texture The texture to apply
         */
        void setTexture(const my::Texture& texture) override;

    public:
        /**
         * @brief Default constructor, creates an empty string.
         *
         * This constructor initializes the text to an empty string, the character size to 0
         * and uses a placeholder font. Before drawing this object you should set a usable font
         * with the setFont() function
         */
        MYGL_EXPORT Text();

        /**
         * @brief Creates a text object
         * @param text A utf-8 encoded std::string with the text to be displayed
         * @param font A my::Font object defining the text's font
         * @param size The character size in pixels
         */
        MYGL_EXPORT Text(const std::string& text, my::Font& font, unsigned int size = 30u);

        /** @brief Default destructor */
        MYGL_EXPORT ~Text() override;

        /**
         * @brief Sets the displayed text's content
         * @param text A utf-8 encoded std::string
         */
        MYGL_EXPORT void setContent(const std::string& text);

        /**
         * @brief Returns the text that is currently displayed
         * @return The text that is currently displayed as a utf-8 encoded std::string
         */
        MYGL_EXPORT const std::string& getContent() const;

        /**
         * @brief Changes the font
         * @param font The new font
         */
        MYGL_EXPORT void setFont(my::Font& font);

        /**
         * @brief Changes the character size
         * @param size The new character size (in pixels)
         */
        MYGL_EXPORT void setFontSize(unsigned int size);
    };

} // namespace my

#endif // MYGL_TEXT
