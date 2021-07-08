#ifndef MYGL_CURSOR
#define MYGL_CURSOR

#include "mygl_export.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <string>

namespace my
{
    /**
     * @brief A class for storing custom cursors
     */
    class MYGL_EXPORT Cursor
    {
    private:
        static std::unordered_map<GLFWcursor*, int> instancesCount;

        GLFWcursor* p_cursor;

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
        Cursor();

        /**
         * @brief Creates a cursor from an image file
         * @param filename The image to use
         * @param xhot The x coordinate of the cursor's hotspot
         * @param yhot The y coordinate of the cursor's hotspot
         */
        Cursor(const std::string& filename, int xhot, int yhot);

        /**
         * @brief Copy constructor
         */
        Cursor(const Cursor& cursor);

        /**
         * @brief Move constructor
         */
        Cursor(Cursor&& cursor);

        ~Cursor();

        /**
         * @brief Copy assignement operator
         */
        Cursor& operator=(const Cursor& cursor);

        /**
         * @brief Move assignement operator
         */
        Cursor& operator=(Cursor&& cursor);

        /**
         * @brief Loads a cursor fron an image file
         * @param filename The image to use
         * @param xhot The x coordinate of the cursor's hotpoint
         * @param yhot The y coordinate of the cursor's hotpoint
         * @return true if the cursor was successfully created
         */
        bool load(const std::string& filename, int xhot, int yhot);

        /**
         * @brief Tells wether the cursor is usable
         * @return true if the cursor can be used
         */
        bool isUsable() const;

        friend class Window;
    };
} // namespace my

#endif // MYGL_CURSOR