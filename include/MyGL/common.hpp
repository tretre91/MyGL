/** @file */

#ifndef MYGL_COMMON
#define MYGL_COMMON

#include "mygl_export.h"

/**
 * @brief Namespace containing every class, function and enum of the library
 */
namespace my
{
    /**
     * @brief Initializes GLFW
     *
     * If glfw's initialization failed the program will exit.
     * Calling this function after GLFW has been successfully initialized will
     * have no effect.
     */
    void initGLFW();

    /**
     * @brief Helper function for static variables relying on glfw functions
     *
     * This function should be used to call glfw functions when it is unsure
     * whether glfw was already initialized (for example in static variables
     * definitions), it ensures that glfw is initialized before calling the
     * function.
     *
     * @tparam GLFWfunction
     * @tparam ...Args
     * @param function The function to call
     * @param ...args The arguments of the function call
     * @return The return value of 'function' called with 'args'
     */
    template<typename GLFWfunction, typename... Args>
    inline auto glfwFunction(GLFWfunction function, Args... args) {
        initGLFW();
        return function(args...);
    }
} // namespace my

#endif // MYGL_COMMON
