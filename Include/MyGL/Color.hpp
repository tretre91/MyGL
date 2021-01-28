#ifndef MY_COLOR
#define MY_COLOR

#include "Header.hpp"

namespace my
{
    class Color {
    public:
        /**
         * @brief The red component
        */
        uint8_t r;

        /**
         * @brief The green component
        */
        uint8_t g;

        /**
         * @brief The blue component
        */
        uint8_t b;

        /**
         * @brief The alpha (transparency) component
        */
        uint8_t alpha;

        static const Color white;
        static const Color black;
        static const Color red;
        static const Color blue;
        static const Color green;

        /**
         * @brief Default constructor, creates a white color
        */
        Color();

        /**
         * @brief Creates a color from individual componenets
         * @param r The color's red component
         * @param g The color's green component
         * @param b The color's blue component
         * @param alpha The transparency component, by default 255 (0 = transparent, 255 = opaque)
        */
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha = 255);

        /**
         * @brief Creates a color from an html color code
         * @param hexColor The html color code (without the '#')
         * @param alpha The transparency component, by default 255 (0 = transparent, 255 = opaque)
        */
        Color(const std::string& hexColor, uint8_t alpha = 255);

        Color& operator=(const Color& color);
    };

    bool operator==(const my::Color& color1, const my::Color& color2);
    bool operator!=(const my::Color& color1, const my::Color& color2);
}

#endif //MY_COLOR