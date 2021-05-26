#include <MyGL/Window.hpp>
#include <iostream>
#include <thread>
using namespace my;

bool GLWindow::gladIsInitialized = false;
unsigned int GLWindow::instancesCount = 0;
std::unordered_map<GLFWwindow*, my::GLWindow*> GLWindow::windows;
my::Camera GLWindow::defaultCamera;

void GLWindow::myglErrorCallback(int error, const char* description) {
    std::cerr << "ERROR::GLFW: " << description;
    std::cerr.flush();
}

GLWindow::GLWindow() : GLWindow(800, 600, "Default") {}

GLWindow::GLWindow(int width, int height, const std::string& title, unsigned short aa) :
  m_projection(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), 0.1f, 10.0f)), p_camera(&defaultCamera), p_window(nullptr),
  m_size(width, height), m_usable(false), m_frameDelay(my::seconds::zero()), m_frametime(1.0) {
    if (instancesCount++ == 0) {
        glfwSetErrorCallback(my::GLWindow::myglErrorCallback);
        if (glfwInit() == GLFW_FALSE) {
            std::cerr << "ERROR::GLFW: Failed to initialize Video module" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, aa);

    p_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (p_window == nullptr) {
        std::cerr << "ERROR::GLFW: Failed to create a window" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(p_window);
    if (!gladIsInitialized) {
        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
            std::cerr << "ERROR::GLAD: Failed to initialize GLAD" << std::endl;
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
    m_chrono = std::chrono::high_resolution_clock::now();

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLWindow::~GLWindow() {
    instancesCount--;
    if (m_usable) {
        windows.erase(p_window);
        glfwDestroyWindow(p_window);
    }
    if (instancesCount == 0) {
        glfwTerminate();
    }
}

bool GLWindow::isRunning() const {
    return m_usable;
}

void GLWindow::close() {
    if (m_usable) {
        windows.erase(p_window);
        glfwDestroyWindow(p_window);
        m_usable = false;
    }
}

bool GLWindow::pollEvent(my::Event& e) {
    glfwPollEvents();
    if (m_eventQueue.empty()) {
        return false;
    }
    e = m_eventQueue.front();
    m_eventQueue.pop_front();
    return true;
}

void GLWindow::setActive() {
    glfwMakeContextCurrent(p_window);
}

void GLWindow::setFramerate(unsigned int limit) {
    enableVsync(false);
    m_frameDelay = limit > 0 ? my::seconds(1.0 / limit) : my::seconds::zero();
}

void GLWindow::enableVsync(bool enable) {
    setActive();
    if (enable) {
        glfwSwapInterval(1);
        m_frameDelay = my::seconds::zero();
    } else {
        glfwSwapInterval(0);
    }
}

void GLWindow::clear(const my::Color& color) const {
    glm::vec4 nColor = color.getNormalized();
    glClearColor(nColor.r, nColor.g, nColor.b, nColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLWindow::setClipPlanes(int left, int right, int bottom, int top) {
    m_projection = glm::ortho(static_cast<float>(left), static_cast<float>(right), static_cast<float>(bottom), static_cast<float>(top), 0.1f, 10.0f);
}

void GLWindow::setViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void GLWindow::setCamera(my::Camera& camera) {
    p_camera = &camera;
}

my::Camera& GLWindow::getCamera() {
    return *p_camera;
}

void GLWindow::setSize(unsigned int width, unsigned int height, bool resizeViewport) {
    m_size.x = static_cast<int>(width);
    m_size.y = static_cast<int>(height);
    if (m_usable) {
        glfwSetWindowSize(p_window, m_size.x, m_size.y);
    }
    if (resizeViewport) {
        setClipPlanes(0, m_size.x, 0, m_size.y);
        setViewport(0, 0, m_size.x, m_size.y);
    }
}

glm::ivec2 GLWindow::getSize() const {
    return m_size;
}

void GLWindow::draw(my::AbstractShape& shape) {
    my::AbstractShape* ptr = &shape;
    ptr->draw(p_camera->lookAt(), m_projection);
}

void GLWindow::display() const {
    if (m_usable) {
        glfwSwapBuffers(p_window);
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = currentTime - m_chrono;
        if (elapsed < m_frameDelay) {
            std::this_thread::sleep_for(m_frameDelay - elapsed);
        }
        currentTime = std::chrono::high_resolution_clock::now();
        m_frametime = currentTime - m_chrono;
        m_chrono = currentTime;
    }
}

double GLWindow::getFrametime() const {
    return m_frametime.count();
}

void* GLWindow::getGLProcAdress(const char* name) {
    return (void*)glfwGetProcAddress(name);
}

/* Callback functions related to the event system */

void GLWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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

    e.mods = {(mods & 0x01) > 0, (mods & 0x02) > 0, (mods & 0x04) > 0, (mods & 0x08) > 0, (mods & 0x10) > 0, (mods & 0x20) > 0};

    windows[window]->m_eventQueue.push_back(e);
}

void GLWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    my::Event e;
    if (action == GLFW_PRESS) {
        e.type = my::EventType::mouseButtonPressed;
    } else {
        e.type = my::EventType::mouseButtonReleased;
    }

    int windowHeight;
    glfwGetWindowSize(window, nullptr, &windowHeight);
    double cursor_x, cursor_y;
    glfwGetCursorPos(window, &cursor_x, &cursor_y);

    e.mouse.pos.x = static_cast<float>(cursor_x);
    e.mouse.pos.y = static_cast<float>(windowHeight - cursor_y);

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

    e.mods = {(mods & 0x01) > 0, (mods & 0x02) > 0, (mods & 0x04) > 0, (mods & 0x08) > 0, (mods & 0x10) > 0, (mods & 0x20) > 0};

    windows[window]->m_eventQueue.push_back(e);
}

void GLWindow::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    my::Event e;
    int windowHeight;
    glfwGetWindowSize(window, nullptr, &windowHeight);
    e.type = my::EventType::mouseMoved;
    e.mouse.button = my::MouseButton::none;
    e.mouse.pos = glm::vec2(static_cast<float>(xpos), static_cast<float>(windowHeight - ypos));
    windows[window]->m_eventQueue.push_back(e);
}

void GLWindow::cursorEnterCallback(GLFWwindow* window, int entered) {
    my::Event e;
    e.keyCode = my::Key::unknown;
    if (entered == GLFW_TRUE) {
        e.type = my::EventType::cursorEntered;
    } else {
        e.type = my::EventType::cursorLeft;
    }
    windows[window]->m_eventQueue.push_back(e);
}

void GLWindow::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    my::Event e;
    e.type = my::EventType::mouseScrolled;
    int deltax = -static_cast<int>(xoffset);
    int deltay = static_cast<int>(yoffset);
    e.scrollOffset = glm::ivec2(deltax, deltay);
    windows[window]->m_eventQueue.push_back(e);
}

void GLWindow::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    my::Event e;
    e.type = my::EventType::windowResized;
    e.windowSize = glm::ivec2(width, height);
    windows[window]->m_size = e.windowSize;
    windows[window]->m_eventQueue.push_back(e);
}

void GLWindow::windowCloseCallback(GLFWwindow* window) {
    my::Event e;
    e.type = my::EventType::windowShouldClose;
    e.keyCode = my::Key::unknown;
    windows[window]->m_eventQueue.push_back(e);
    glfwSetWindowShouldClose(window, GLFW_FALSE);
}
