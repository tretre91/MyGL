#include <MyGL/common.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#ifdef _WIN32
#    include <Windows.h>

unsigned int getLowestTimerPeriod() {
    TIMECAPS timer;
    timeGetDevCaps(&timer, sizeof(timer));
    return timer.wPeriodMin;
}

const unsigned int period = getLowestTimerPeriod();

#else
#    include <cerrno>
#    include <ctime>
#endif

namespace my
{
    void myglErrorCallback(int error, const char* description) {
        std::cerr << "ERROR::GLFW: " << description;
    }

    void initGLFW() {
        if (!glfwInit()) {
            std::cerr << "ERROR::GLFW: Failed to initialize Video module\n";
            exit(EXIT_FAILURE);
        }
        glfwSetErrorCallback(myglErrorCallback);
    }

    void sleep(unsigned long long nanoseconds) {
#ifdef _WIN32
        timeBeginPeriod(period);
        Sleep(nanoseconds / 1000000);
        timeEndPeriod(period);
#else
        timespec required, remaining;
        required.tv_sec = nanoseconds / 1000000000;
        required.tv_nsec = nanoseconds % 1000000000;

        while (nanosleep(&required, &remaining) == -1) {
            if (errno != EINTR)
                break;
            required = remaining;
        }
#endif
    }

} // namespace my
