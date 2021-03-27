#ifndef MY_WINDOW
#define MY_WINDOW

#include "mygl_export.h"

//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Event.hpp"
#include "Color.hpp"
#include "Animation.hpp"
#include "Drawable/AbstractShape.hpp"
#include "Camera/FixedCamera.hpp"

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
        static std::unordered_map<GLFWwindow*, std::deque<Event>*> eventQueues;

        glm::mat4 m_projection;
        my::FixedCamera* p_camera;
        GLFWwindow* p_window;
        bool m_usable;
        my::seconds m_frameDelay;
        mutable my::seconds m_frametime;
        mutable std::chrono::time_point<std::chrono::high_resolution_clock> m_chrono;
        std::deque<Event> m_eventQueue;

        /**
         * @brief Initializes glad from outside of the DLL
         * @return true if the initialization was successful
        */
        inline bool initGlad() {
            return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        }

        /**
         * @brief Error callback for GLFW errors
         * @param error The error code
         * @param description The error description
        */
        static void myglErrorCallback(int error, const char* description);

        /**
         * @brief Callback function called when a GLWindow is resized
         * @param window The pointer to the underlying GLFWwindow
         * @param width The new width in pixels
         * @param height The new height in pixels
        */
        static void myglFramebufferSizeCallback(GLFWwindow* window, int width, int height);

        // TODO: add a doxygen group for the event callbacks

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void cursorEnterCallback(GLFWwindow* window, int entered);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

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
         * 
         * If the user tried to close the window (clicking the closing button or
         * pressing Alt + f4 for example), a call to this method will close the window
         * and return false
         * 
         * @return true if the window is opened
        */
        bool isRunning();

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
         * @brief Sets the camera used by this window
         * @param camera The camera which will be used to see the window's content
        */
        void setCamera(my::FixedCamera& camera);

        /**
         * @brief Sets the projection used by the window (This method should be replaced by an
                  initilization in the constructor)
         * @param projection The projection matrix used (orthographic or perspective)
        */
        void setProjection(const glm::mat4& projection);

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