#ifndef MYGL_WINDOW
#define MYGL_WINDOW

#include "mygl_export.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Camera.hpp"
#include "Color.hpp"
#include "Cursor.hpp"
#include "Drawable/AbstractShape.hpp"
#include "Event.hpp"
#include "Image.hpp"

#include <chrono>
#include <deque>
#include <unordered_map>

namespace my
{
    /**
     * @brief Flags passed when creating a window to change its behaviour
     */
    enum WindowFlag : unsigned int
    {
        none = 0,
        /**
         * @brief Makes the window resizable
         */
        resizable = 1 << 0,
        /**
         * @brief Keeps the window on top of other windows
         */
        floating = 1 << 1,
        /**
         * @brief Allows the framebuffer to be transparent, this means that calling
         * window.clear() with a color whose aplha value is below 255 will result
         * in a transparent background (the opacity will depend on the alpha value)
         */
        transparent = 1 << 2,
        /**
         * @brief Maximizes the window when created
         */
        maximized = 1 << 3
    };

    /**
     * @brief Class for creating a window
     */
    class MYGL_EXPORT Window
    {
    private:
        static bool gladIsInitialized;
        static unsigned int instancesCount;
        static std::unordered_map<GLFWwindow*, Window*> windows;
        static my::Camera defaultCamera;

        glm::mat4 m_projection;
        my::Camera* p_camera;
        GLFWwindow* p_window;
        my::Cursor m_cursor;
        glm::ivec2 m_size;
        bool m_usable;
        std::chrono::microseconds m_frameDelay;
        mutable std::chrono::microseconds m_frametime;
        mutable std::chrono::time_point<std::chrono::steady_clock> m_chrono;
        std::deque<Event> m_eventQueue;

        /** @name Event callbacks
         * @brief Event callback functions for handling glfw events
         */
        /** @{ */
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void cursorEnterCallback(GLFWwindow* window, int entered);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void windowCloseCallback(GLFWwindow* window);
        /** @} */

    public:
        /**
         * @brief Default constructor, creates a 800 * 600 window
         */
        Window();

        /**
         * @brief Creates a window
         * @param width The window's width
         * @param height The window's height
         * @param title The window's title
         * @param flags A combination of WindowFlags, you can specify multiple properties
         *              using a bitwise or, for example if you want your window to be resizable
         *              and to always be on top you would pass "my::resizable | my::floating"
         *              as the flags parameter
         * @param antiAliasing The anti aliasing level, should be 0 (disabled), 2, 4 or 8.
         *                     Defaults to 0
         * @param GLVersionMajor The minor version of the desired OpenGL version, supported
         *                       OpenGL versions are 3.3 and 4.0 to 4.6
         * @param GLVersionMinor The minor version of the desired OpenGL version, supported
         *                       OpenGL versions are 3.3 and 4.0 to 4.6
         */
        Window(int width, int height, const std::string& title, unsigned int flags = my::WindowFlag::none, int antiAliasing = 0, int GLVersionMajor = 3,
          int GLVersionMinor = 3);

        ~Window();

        /**
         * @brief Tells wheter a window is opened or closed
         * @return true if the window is opened, false if it is closed
         */
        bool isRunning() const noexcept;

        /**
         * @brief Closes a window
         */
        void close();

        /**
         * @brief retrieves the event in front of the event queue
         * @param e A my::Event which will hold the event's value
         * @return false if there was no event in the queue, true otherwise
         */
        bool pollEvent(my::Event& e);

        /**
         * @brief Sets the window's OpenGL context current
         */
        void setActive() noexcept;

        /**
         * @brief Limit the window's framerate at the specified value, a call
         *        to this method will disable vsync if it was previously enabled
         * @param limit The framerate limit, 0 means unlimited
         */
        void setFramerate(unsigned int limit);

        /**
         * @brief Enable or disable vsync, enabling vsync will override the framerate
         *        limit if it was previously set
         * @param enable True to enable vsync, false to disable it
         */
        void enableVsync(bool enable);

        /**
         * @brief Clears the window's content and replaces it with a background color
         * @param color The background color
         */
        void clear(const my::Color& color) const;

        /**
         * @brief Sets the dimensions of the window's projection matrix's frustum
         *
         * @param left The frustum's left coordinate
         * @param right The frustum's tight coordinate
         * @param bottom The frustum's bottom coordinate
         * @param top The frustum's top coordinate
         */
        void setClipPlanes(int left, int right, int bottom, int top);

        /**
         * @brief Sets the viewport's position and size
         *
         * This is equivalent to calling glViewport(left, bottom, width, height)
         * @param x The viewport's lower left corner's x position
         * @param y The viewport's lower left corner's y position
         * @param width The viewport's width
         * @param height The viewport's height
         */
        void setViewport(int x, int y, int width, int height);

        /**
         * @brief Sets the camera used by this window
         * @param camera The camera which will be used to see the window's content
         */
        void setCamera(my::Camera& camera) noexcept;

        /**
         * @brief Returns the currently bound camera
         * @return A reference to the camera which is currently attached to the
         *         window
         */
        my::Camera& getCamera() noexcept;

        /**
         * @brief Sets the size of the window's rendering area
         *
         * This function only modifies the size of the window, you should then
         * handle the size of the projection matrix and
         * @param width The new width
         * @param height The new height
         * @param resizeViewport If true, the function will set the clip planes and viewport
         *                       to the same size as the window (with a call to setClipPlanes(0, width, 0, height)
         *                       and setViewport(0, 0, width, height)
         */
        void setSize(unsigned int width, unsigned int height, bool resizeViewport = false);

        /**
         * @brief Gives the window's size
         * @return The size of the window's viewport (rendering area) in pixels
         */
        glm::ivec2 getSize() const noexcept;

        /**
         * @brief Sets the window's icon
         * @param icon The image to set as the new window icon, it must have 4
         *        channels (RGBA). Passing a default constructed or an invalid
         *        Image will revert to the default window icon
         */
        void setIcon(const Image& icon);

        /**
         * @brief Sets the cursor used by this window
         * @param cursor The new cursor to use
         */
        void setCursor(const Cursor& cursor) noexcept;

        /**
         * @brief Draws a shape
         * @param shape The shape to draw, it must inherit from AbstractShape
         */
        void draw(my::AbstractShape& shape) const;

        /**
         * @brief Refreshes the window to display all the things which have been drawn
         */
        void display() const;

        /**
         * @brief Indicates the time that the last frame took to render
         * @return The window's frametime in seconds
         */
        double getFrametime() const;

        static void* getGLProcAdress(const char* name);
    };

} // namespace my

#endif // MYGL_WINDOW