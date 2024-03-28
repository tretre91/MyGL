#ifndef MYGL_COLOR
#define MYGL_COLOR

#include "mygl_export.h"

#include <glm/glm.hpp>

#include <string>

namespace my
{
    /**
     * @brief Class for storing color objects
     */
    class Color
    {
    private:
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t alpha;
        glm::vec4 normalized;

    public:
        MYGL_EXPORT static const Color white;
        MYGL_EXPORT static const Color black;
        MYGL_EXPORT static const Color red;
        MYGL_EXPORT static const Color green;
        MYGL_EXPORT static const Color blue;

        /**
         * @brief Default constructor, creates a white color
         */
        MYGL_EXPORT Color() noexcept;

        /**
         * @brief Creates a color from individual componenets
         * @param r The color's red component
         * @param g The color's green component
         * @param b The color's blue component
         * @param alpha The transparency component, by default 255 (0 = transparent, 255 = opaque)
         */
        MYGL_EXPORT Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha = 255) noexcept;

        /**
         * @brief Creates a color from an hexadecimal color code
         * @param hexColor The hex color code (without the '#')
         * @param alpha The transparency component, by default 255 (0 = transparent, 255 = opaque)
         */
        MYGL_EXPORT Color(const std::string& hexColor, uint8_t alpha = 255);

        /**
         * @brief Gives the color as a glm::ivec4
         * @return A glm::ivec4 conataining the color's components
         */
        MYGL_EXPORT glm::ivec4 get() const noexcept;

        /**
         * @brief Gives the color with its components normalized (float between
         *        0 and 1)
         * @return A glm::vec4 containing the color's noramlized components
         */
        MYGL_EXPORT glm::vec4 getNormalized() const noexcept;

        /**
         * @return The color's red component
         */
        MYGL_EXPORT int getRed() const noexcept;

        /**
         * @return The color's green component
         */
        MYGL_EXPORT int getGreen() const noexcept;

        /**
         * @return The color's blue component
         */
        MYGL_EXPORT int getBlue() const noexcept;

        /**
         * @return The color's alpha component
         */
        MYGL_EXPORT int getAlpha() const noexcept;

        /**
         * @brief Sets the color's red component
         * @param red The new red component
         */
        MYGL_EXPORT void setRed(uint8_t red) noexcept;

        /**
         * @brief Sets the color's green component
         * @param green The new green component
         */
        MYGL_EXPORT void setGreen(uint8_t green) noexcept;

        /**
         * @brief Sets the color's blue component
         * @param blue The new blue component
         */
        MYGL_EXPORT void setBlue(uint8_t blue) noexcept;

        /**
         * @brief Sets the color's alpha component
         * @param alpha The new alpha component
         */
        MYGL_EXPORT void setAlpha(uint8_t alpha) noexcept;
    };

} // namespace my

MYGL_EXPORT bool operator==(const my::Color& color1, const my::Color& color2) noexcept;
MYGL_EXPORT bool operator!=(const my::Color& color1, const my::Color& color2) noexcept;

#endif // MYGL_COLOR