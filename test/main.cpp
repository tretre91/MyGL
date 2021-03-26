#include <MyGL/Window.hpp>
#include <MyGL/Drawable.hpp>
#include <MyGL/Camera.hpp>

#include <iostream>

bool moveLeft = false;
bool moveRight = false;
bool moveUp = false;
bool moveDown = false;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key)
        {
        case GLFW_KEY_LEFT:
            moveLeft = true;
            break;

        case GLFW_KEY_RIGHT:
            moveRight = true;
            break;

        case GLFW_KEY_UP:
            moveUp = true;
            break;

        case GLFW_KEY_DOWN:
            moveDown = true;
            break;
        }
    } else if (action == GLFW_RELEASE) {
        switch (key)
        {
        case GLFW_KEY_LEFT:
            moveLeft = false;
            break;

        case GLFW_KEY_RIGHT:
            moveRight = false;
            break;

        case GLFW_KEY_UP:
            moveUp = false;
            break;

        case GLFW_KEY_DOWN:
            moveDown = false;
            break;
        }
    }
}

int main() {
    my::GLWindow window(800, 600, "Test");
    window.setFramerate(75);
    glfwSetKeyCallback(window.p_window, keyCallback);

    my::Cam2D camera(0, 0);
    camera.setSpeed(100.0f);
    window.setCamera(camera);

    my::Rectangle rect(100, 50, 400, 300);
    rect.setColor(175, 87, 43, 255);

    my::Polygon<20> poly(50, 100, 100);
    poly.setColor(my::Color::white);

    my::Font openSans("@RESSOURCES_DIR@/Fonts/OpenSans-Regular.ttf");
    my::Text text("The .\\quick \"brown\" {fox} \n#jumps [over] the lazy dog!", openSans, 60);
    text.setPosition(0, 60);
    text.setOutlineThickness(5);
    text.setColor(my::Color::white);
    text.setOutlineColor(my::Color::red);

    const my::Color clearColor("1E1E1E");

    while (window.isRunning()) {

        float frametime = window.getFrametime();
        if (moveLeft) camera.moveLeft(frametime);
        if (moveRight) camera.moveRight(frametime);
        if (moveUp) camera.moveUp(frametime);
        if (moveDown) camera.moveDown(frametime);

        window.clear(clearColor);
        window.draw(rect);
        window.draw(poly);
        window.draw(text);
        window.display();
        glfwPollEvents();
    }
}