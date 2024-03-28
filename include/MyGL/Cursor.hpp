#ifndef MYGL_CURSOR
#define MYGL_CURSOR

#include "mygl_export.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Image.hpp"
#include <memory>
#include <string>

namespace my
{
    /**
     * @brief A class for storing custom cursors
     */
    class Cursor
    {
    private:
        std::shared_ptr<GLFWcursor> p_cursor;

        /**
         * @brief Creates a cursor from a GLFWcursor
         * @param cursor A pointer to a GLFWcursor
         */
        Cursor(GLFWcursor* cursor);

    public:
        /**
         * @brief The standard arrow cursor, this is the default
         */
        static const Cursor arrow;
        /**
         * @brief The I-beam shaped cursor (or text cursor)
         */
        static const Cursor IBeam;
        /**
         * @brief The crosshair cursor (cross shape)
         */
        static const Cursor crosshair;
        /**
         * @brief The pointing hand cursor
         */
        static const Cursor hand;
        /**
         * @brief The horizontal resize arrow cursor
         */
        static const Cursor horizontalResize;
        /**
         * @brief The vertical resize arrow cursor
         */
        static const Cursor verticalResize;

        /**
         * @brief Creates a default arrow cursor
         */
        MYGL_EXPORT Cursor() noexcept;

        /**
         * @brief Creates a cursor from an image file
         *
         * The image used to create the cursor must be in a RGBA pixel format (4
         * channels).
         * The hotspot is the point defining the cursor's position, its coordinates
         * are relative to the image's lower left corner.
         *
         * @param image The image to use
         * @param xhot The x coordinate of the cursor's hotspot
         * @param yhot The y coordinate of the cursor's hotspot
         */
        MYGL_EXPORT Cursor(const Image& image, int xhot, int yhot);

        MYGL_EXPORT ~Cursor() = default;

        /**
         * @brief Loads a cursor fron an image file
         * @param image The image to use
         * @param xhot The x coordinate of the cursor's hotpoint
         * @param yhot The y coordinate of the cursor's hotpoint
         * @return true if the cursor was successfully created
         */
        MYGL_EXPORT bool load(const Image& image, int xhot, int yhot);

        /**
         * @brief Tells wether the cursor is usable
         * @return true if the cursor can be used
         */
        MYGL_EXPORT bool isUsable() const noexcept;

        friend class Window;
    };
} // namespace my

#endif // MYGL_CURSOR