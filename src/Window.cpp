#include <MyGL/Window.hpp>
#include <MyGL/common.hpp>
#include <iostream>
#include <thread>
using namespace std::chrono_literals;

/**
 * @brief Checks if a given OpenGL version is compatible with the library, and
 *        defaults to the closest compatible version if it's not the case
 * @param major The requested major version
 * @param minor The requested minor version
 */
void checkOpenGLVersion(int& major, int& minor) {
    int validMajor, validMinor;
    if (major <= 3) {
        validMajor = 3;
        validMinor = 3;
    } else {
        validMajor = 4;
        validMinor = minor < 0 ? 0 : (minor > 6 ? 6 : minor);
    }
    if (major != validMajor || minor != validMinor) {
        std::cerr << "ERROR::Window: Invalid OpenGL version requested (" << major << '.' << minor << "), defaulting to " << validMajor << '.' << validMinor
                  << '\n';
    }
    major = validMajor;
    minor = validMinor;
}

namespace my
{
    using time_unit = std::chrono::microseconds;

    bool Window::gladIsInitialized = false;
    unsigned int Window::instancesCount = 0;
    std::unordered_map<GLFWwindow*, my::Window*> Window::windows;
    my::Camera Window::defaultCamera;

    Window::Window() : Window(800, 600, "Default") {}

    Window::Window(int width, int height, const std::string& title, unsigned int flags, int antiAliasing, int GLVersionMajor, int GLVersionMinor) :
      m_projection(glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 0.1f, 10.0f)), p_camera(&defaultCamera), p_window(nullptr),
      m_cursor(Cursor::arrow), m_size(width, height), m_usable(false), m_frameDelay(0s), m_frametime(1s) {
        // we restrict the antiAliasing samples value to be between 0 and 8
        if (antiAliasing < 2) {
            antiAliasing = 0;
        } else if (antiAliasing < 4) {
            antiAliasing = 2;
        } else if (antiAliasing < 8) {
            antiAliasing = 4;
        } else {
            antiAliasing = 8;
        }

        checkOpenGLVersion(GLVersionMajor, GLVersionMinor);

        initGLFW();
        glfwWindowHint(GLFW_SAMPLES, antiAliasing);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLVersionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLVersionMinor);

        glfwWindowHint(GLFW_RESIZABLE, (flags & WindowFlag::resizable) != 0);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, (flags & WindowFlag::transparent) != 0);
        glfwWindowHint(GLFW_FLOATING, (flags & WindowFlag::floating) != 0);
        glfwWindowHint(GLFW_MAXIMIZED, (flags & WindowFlag::maximized) != 0);

        p_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (p_window == nullptr) {
            const char* desc;
            glfwGetError(&desc);
            std::cerr << "ERROR::GLFW: Failed to create a window\n" << desc;
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(p_window);

        if (!gladIsInitialized) {
            if (gladLoadGL(glfwGetProcAddress) == 0) {
                std::cerr << "ERROR::GLAD: Failed to initialize GLAD\n";
                glfwDestroyWindow(p_window);
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            gladIsInitialized = true;
        }

        m_usable = true;
        windows.insert({p_window, this});
        glfwSetKeyCallback(p_window, keyCallback);
        glfwSetMouseButtonCallback(p_window, mouseButtonCallback);
        glfwSetCursorPosCallback(p_window, cursorPosCallback);
        glfwSetCursorEnterCallback(p_window, cursorEnterCallback);
        glfwSetScrollCallback(p_window, scrollCallback);
        glfwSetFramebufferSizeCallback(p_window, framebufferSizeCallback);
        glfwSetWindowCloseCallback(p_window, windowCloseCallback);

        glfwSwapInterval(0);
        m_chrono = std::chrono::steady_clock::now();

        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    Window::~Window() {
        instancesCount--;
        if (m_usable) {
            windows.erase(p_window);
            glfwDestroyWindow(p_window);
        }
        if (instancesCount == 0) {
            glfwTerminate();
        }
    }

    bool Window::isRunning() const noexcept {
        return m_usable;
    }

    void Window::close() {
        if (m_usable) {
            windows.erase(p_window);
            glfwDestroyWindow(p_window);
            m_usable = false;
        }
    }

    bool Window::pollEvent(my::Event& e) {
        glfwPollEvents();
        if (m_eventQueue.empty()) {
            return false;
        }
        e = m_eventQueue.front();
        m_eventQueue.pop_front();
        return true;
    }

    void Window::setActive() noexcept {
        glfwMakeContextCurrent(p_window);
    }

    void Window::setFramerate(unsigned int limit) {
        enableVsync(false);
        m_frameDelay = limit > 0 ? time_unit{1s} / limit : 0s;
    }

    void Window::enableVsync(bool enable) {
        setActive();
        if (enable) {
            glfwSwapInterval(1);
            m_frameDelay = time_unit::zero();
        } else {
            glfwSwapInterval(0);
        }
    }

    void Window::clear(const my::Color& color) const {
        glm::vec4 nColor = color.getNormalized();
        glClearColor(nColor.r, nColor.g, nColor.b, nColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void Window::setClipPlanes(float left, float right, float bottom, float top) {
        constexpr float zNear = 0.1f;
        constexpr float zFar = 10.0f;
        m_projection = glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    void Window::setViewport(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
    }

    void Window::setCamera(my::Camera& camera) noexcept {
        p_camera = &camera;
    }

    my::Camera& Window::getCamera() noexcept {
        return p_camera != nullptr ? *p_camera : defaultCamera;
    }

    void Window::setSize(unsigned int width, unsigned int height, bool resizeViewport) {
        m_size.x = static_cast<int>(width);
        m_size.y = static_cast<int>(height);
        if (m_usable) {
            glfwSetWindowSize(p_window, m_size.x, m_size.y);
        }
        if (resizeViewport) {
            setClipPlanes(0.0f, static_cast<float>(m_size.x), static_cast<float>(m_size.y), 0.0f);
            setViewport(0, 0, m_size.x, m_size.y);
        }
    }

    glm::ivec2 Window::getSize() const noexcept {
        return m_size;
    }

    void Window::setIcon(const Image& image) {
        if (!image.isUsable()) {
            glfwSetWindowIcon(p_window, 0, nullptr);
        } else {
            GLFWimage icon;
            icon.width = static_cast<int>(image.getWidth());
            icon.height = static_cast<int>(image.getHeight());
            icon.pixels = image.data();
            glfwSetWindowIcon(p_window, 1, &icon);
        }
    }

    void Window::setCursor(const Cursor& cursor) noexcept {
        if (cursor.isUsable()) {
            m_cursor = cursor;
            glfwSetCursor(p_window, m_cursor.p_cursor.get());
        }
    }

    void Window::draw(my::AbstractShape& shape) const {
        shape.draw(p_camera->lookAt(), m_projection);
    }

    void Window::display() const {
        if (m_usable) {
            auto currentTime = std::chrono::steady_clock::now();
            const time_unit elapsed = std::chrono::duration_cast<time_unit>(currentTime - m_chrono);
            if (elapsed < m_frameDelay) {
                std::this_thread::sleep_for(m_frameDelay - elapsed);
            }
            glfwSwapBuffers(p_window);
            currentTime = std::chrono::steady_clock::now();
            m_frametime = std::chrono::duration_cast<time_unit>(currentTime - m_chrono);
            m_chrono = currentTime;
        }
    }

    double Window::getFrametime() const {
        return std::chrono::duration_cast<std::chrono::duration<double>>(m_frametime).count();
    }

    void* Window::getGLProcAdress(const char* name) {
        return (void*)glfwGetProcAddress(name);
        // return reinterpret_cast<void*>(glfwGetProcAddress(name));
    }

    /* Callback functions related to the event system */

    void Window::keyCallback(GLFWwindow* window, int key, int /* scancode */, int action, int mods) {
        my::Event e;
        switch (action) {
        case GLFW_PRESS:
            e.type = my::EventType::keyPressed;
            break;

        case GLFW_RELEASE:
            e.type = my::EventType::keyReleased;
            break;

        case GLFW_REPEAT:
            e.type = my::EventType::keyRepeated;
            break;
        }

        e.keyCode = static_cast<my::Key>(key);
        
        e.mods.shift     = mods & 0x01;
        e.mods.ctrl      = mods & 0x02;
        e.mods.alt       = mods & 0x04;
        e.mods.os        = mods & 0x08;
        e.mods.caps_lock = mods & 0x10;
        e.mods.num_lock  = mods & 0x20;

        windows[window]->m_eventQueue.push_back(e);
    }

    void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        my::Event e;
        if (action == GLFW_PRESS) {
            e.type = my::EventType::mouseButtonPressed;
        } else {
            e.type = my::EventType::mouseButtonReleased;
        }

        double cursor_x, cursor_y;
        glfwGetCursorPos(window, &cursor_x, &cursor_y);

        e.mouse.pos.x = static_cast<float>(cursor_x);
        e.mouse.pos.y = static_cast<float>(cursor_y);

        switch (button) {
        case GLFW_MOUSE_BUTTON_1:
            e.mouse.button = my::MouseButton::left;
            break;

        case GLFW_MOUSE_BUTTON_2:
            e.mouse.button = my::MouseButton::right;
            break;

        case GLFW_MOUSE_BUTTON_3:
            e.mouse.button = my::MouseButton::middle;
            break;

        case GLFW_MOUSE_BUTTON_4:
            e.mouse.button = my::MouseButton::extra_1;
            break;

        case GLFW_MOUSE_BUTTON_5:
            e.mouse.button = my::MouseButton::extra_2;
            break;

        case GLFW_MOUSE_BUTTON_6:
            e.mouse.button = my::MouseButton::extra_3;
            break;

        case GLFW_MOUSE_BUTTON_7:
            e.mouse.button = my::MouseButton::extra_4;
            break;

        case GLFW_MOUSE_BUTTON_8:
            e.mouse.button = my::MouseButton::extra_5;
            break;

        default:
            break;
        }

        e.mods.shift     = mods & 0x01;
        e.mods.ctrl      = mods & 0x02;
        e.mods.alt       = mods & 0x04;
        e.mods.os        = mods & 0x08;
        e.mods.caps_lock = mods & 0x10;
        e.mods.num_lock  = mods & 0x20;

        windows[window]->m_eventQueue.push_back(e);
    }

    void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
        my::Event e;
        e.type = my::EventType::mouseMoved;
        e.mouse.button = my::MouseButton::none;
        e.mouse.pos = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
        windows[window]->m_eventQueue.push_back(e);
    }

    void Window::cursorEnterCallback(GLFWwindow* window, int entered) {
        my::Event e;
        e.keyCode = my::Key::unknown;
        if (entered == GLFW_TRUE) {
            e.type = my::EventType::cursorEntered;
        } else {
            e.type = my::EventType::cursorLeft;
        }
        windows[window]->m_eventQueue.push_back(e);
    }

    void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        my::Event e;
        e.type = my::EventType::mouseScrolled;
        int deltax = -static_cast<int>(xoffset);
        int deltay = static_cast<int>(yoffset);
        e.scrollOffset = glm::ivec2(deltax, deltay);
        windows[window]->m_eventQueue.push_back(e);
    }

    void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        my::Event e;
        e.type = my::EventType::windowResized;
        e.windowSize = glm::ivec2(width, height);
        windows[window]->m_size = e.windowSize;
        windows[window]->m_eventQueue.push_back(e);
    }

    void Window::windowCloseCallback(GLFWwindow* window) {
        my::Event e;
        e.type = my::EventType::windowShouldClose;
        e.keyCode = my::Key::unknown;
        windows[window]->m_eventQueue.push_back(e);
        glfwSetWindowShouldClose(window, GLFW_FALSE);
    }
} // namespace my
