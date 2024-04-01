#include <MyGL/common.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>


namespace my
{
    void myglErrorCallback(int error, const char* description) {
        std::cerr << "ERROR::GLFW: (" << error << ") " << description;
    }

    void initGLFW() {
        if (!glfwInit()) {
            std::cerr << "ERROR::GLFW: Failed to initialize Video module\n";
            exit(EXIT_FAILURE);
        }
        glfwSetErrorCallback(myglErrorCallback);
    }
} // namespace my
