#ifndef MY_WINDOW
#define MY_WINDOW

#include <glad/glad.h>
#include <SDL.h>

#include "mygl_export.h"

#include "Header.hpp"
#include "Color.hpp"
#include "Animation.hpp"
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
        SDL_GLContext m_glContext;
        mutable float m_frametime;
        mutable Uint32 m_tickCount;
        Uint32 m_frameDelay;

        /**
         * @brief Initializes glad from outside of the DLL
         * @return true if the initialization was successful
        */
        inline bool initGlad() {
            return gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
        }

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

        bool setActive(bool activate);

        /**
         * @brief Limit the window's framerate at the specified value, a call
         *        to this method will disable vsync if it was previously enabled
         * @param limit The framerate limit, 0 means unlimited
        */ 
        void setFramerate(unsigned int limit);

        /**
         * @brief Enable or disable vsync, this function will try to use adaptative
         *        sync if avalaible, and default to vsync if not. A call to this method
         *        will override the framerate limit if previously set
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
        float getFrametime() const;

        static void* getGLProcAdress(const char* name);
    };
}

#endif //MY_WINDOW