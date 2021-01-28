#ifndef MY_WINDOW
#define MY_WINDOW

#include <SFML/Window.hpp>
#include "Header.hpp"
#include "Drawable/AbstractShape.hpp"
#include "Camera/FixedCamera.hpp"

namespace my
{
    /**
     * @brief Class for creating a Window
    */
    class GLWindow : public sf::Window {
    private:
        static bool gladIsInitialized;
        glm::mat4 m_projection;
        my::FixedCamera* m_camera;

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

        /**
         * @brief Creates a window with an sf::RenderWindow style contructor (will probably be removed later)
         * @param mode The sf::VideoMode to use
         * @param title The window's title
         * @param style The sf::Style of the window
        */
        GLWindow(sf::VideoMode mode, const std::string& title, sf::Uint32 style = sf::Style::Default);

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
    };
}

#endif //MY_WINDOW