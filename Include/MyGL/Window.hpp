#ifndef MY_WINDOW
#define MY_WINDOW

#include <glad/glad.h>
#include <SDL.h>

#include "mygl_export.h"

#include "Header.hpp"
#include "Color.hpp"
#include "Drawable/AbstractShape.hpp"
#include "Camera/FixedCamera.hpp"

namespace my
{
    /**
     * @brief Class for creating a Window
    */
    class MYGL_EXPORT GLWindow {
    private:
        static bool gladIsInitialized;
        static unsigned int instancesCount;
        glm::mat4 m_projection;
        my::FixedCamera* p_camera;
        SDL_Window* p_window;
        SDL_GLContext glContext;

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
        */
        GLWindow(int width, int height, const std::string& title);

        //GLWindow(int width, int height, const std::string& title, int glMajorVer, int glMinorVer); TODO

        ~GLWindow();

        bool setActive(bool activate);

        //void setFramerate(unsigned int limit); TODO

        /**
         * @brief Enable or disable vsync, this function will try to use adaptative
         *        sync if avalaible, and default to vsync if not
         * @param enable True to enable vsync, false to disable it
        */
        void enableVsync(bool enable) const;

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
         * @brief Refreshes the window tto display all the things which have been drawn
        */
        void display() const;

        static void* getGLProcAdress(const char* name);
    };
}

#endif //MY_WINDOW