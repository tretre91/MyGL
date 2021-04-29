#ifndef MY_WINDOW
#define MY_WINDOW

#include "mygl_export.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Event.hpp"
#include "Color.hpp"
#include "Animation.hpp"
#include "Drawable/AbstractShape.hpp"
#include "Camera.hpp"

#include <thread>
#include <chrono>
#include <deque>
#include <unordered_map>

namespace my
{
    using seconds = std::chrono::duration<double, std::ratio<1>>;
    
    /**
     * @brief Class for creating a Window
    */
    class MYGL_EXPORT GLWindow {
    private:
        static bool gladIsInitialized;
        static unsigned int instancesCount;
        static std::unordered_map<GLFWwindow*, GLWindow*> windows;
        static my::Camera defaultCamera;

        glm::mat4 m_projection;
        my::Camera* p_camera;
        GLFWwindow* p_window;
        glm::ivec2 m_size;
        bool m_usable;
        my::seconds m_frameDelay;
        mutable my::seconds m_frametime;
        mutable std::chrono::time_point<std::chrono::high_resolution_clock> m_chrono;
        std::deque<Event> m_eventQueue;

        /**
         * @brief Error callback for GLFW errors
         * @param error The error code
         * @param description The error description
        */
        static void myglErrorCallback(int error, const char* description);

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
        GLWindow();

        /**
         * @brief Creates a width * height window, with a title
         * @param width The window's width
         * @param height The window's height
         * @param title The window's title
         * @param aa The anti aliasing level, should be 0 (disabled), 2, 4 or 8. Disabled if not
         *           specified
        */
        GLWindow(int width, int height, const std::string& title, unsigned short aa = 0);

        //GLWindow(int width, int height, const std::string& title, int glMajorVer, int glMinorVer); TODO

        ~GLWindow();

        /**
         * @brief Tells wheter a window is opened or closed
         * @return true if the window is opened, false if it is closed
        */
        bool isRunning() const;

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
         * @brief Sets window's OpenGL context current
        */
        void setActive();

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
        void setCamera(my::Camera& camera);

        /**
         * @brief Returns the currently bound camera
         * @return A reference to the camera which is currently attached to the
         *         window
        */
        my::Camera& getCamera();

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
        glm::ivec2 getSize() const;

        /**
         * @brief Draws a shape
         * @param shape The shape to draw, it must inherit from AbstractShape
        */
        void draw(my::AbstractShape& shape);

        /**
         * @brief Updates and draws an animated shape
         * @param anim The animation to be drawn
        */
        void draw(my::Animation& anim);

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
}

#endif //MY_WINDOW