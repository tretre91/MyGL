#include <MyGL/Window.hpp>
using namespace my;

bool GLWindow::gladIsInitialized = false;
unsigned int GLWindow::instancesCount = 0;

void GLWindow::myglErrorCallback(int error, const char* description) {
    std::cerr << "ERROR::GLFW: " << description;
    std::cerr.flush();
}

void GLWindow::myglFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    //glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
}

GLWindow::GLWindow() : GLWindow(800, 600, "Default") {}

GLWindow::GLWindow(int width, int height, const std::string& title, unsigned short aa) : 
    m_projection(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), 0.1f, 10.0f)),
    p_camera(nullptr), p_window(nullptr), m_frametime(1.0), m_chrono{}, m_frameDelay(my::seconds::zero())
{
    if (instancesCount++ == 0) {
        glfwSetErrorCallback(my::GLWindow::myglErrorCallback);
        if (!glfwInit()) {
            std::cerr << "ERROR::GLFW: Failed to initialize Video module" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    p_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (p_window == nullptr) {
        std::cerr << "ERROR::GLFW: Failed to create a window" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(p_window);
    if (!gladIsInitialized) {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) || !initGlad()) {
            std::cout << "ERROR::GLAD: Failed to initialize GLAD" << std::endl;
            glfwDestroyWindow(p_window);
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        gladIsInitialized = true;
    }

    glfwSetFramebufferSizeCallback(p_window, myglFramebufferSizeCallback);

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
    if (instancesCount == 0) {
        glfwTerminate();
    }
}

bool GLWindow::isRunning() {
    if (p_window == nullptr) {
        return false;
    } else if (glfwWindowShouldClose(p_window)) {
        glfwDestroyWindow(p_window);
        return false;
    } else {
        return true;
    }
}

void GLWindow::close() {
    glfwSetWindowShouldClose(p_window, true);
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
    glm::vec4 nColor(color.getNormalized());
    glClearColor(nColor.r, nColor.g, nColor.b, nColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLWindow::setCamera(my::FixedCamera& camera) {
    p_camera = &camera;
}

void GLWindow::setProjection(const glm::mat4& projection) {
    m_projection = projection;
}

void GLWindow::draw(my::AbstractShape& shape) {
    my::AbstractShape* ptr = &shape;
    ptr->draw(p_camera->lookAt(), m_projection);
}

void my::GLWindow::draw(my::Animation& anim) {
    anim.draw(p_camera->lookAt(), m_projection);
}

void GLWindow::display() const {
    glfwSwapBuffers(p_window);
    auto temp = std::chrono::high_resolution_clock::now();
    auto elapsed = temp - m_chrono;
    if (elapsed < m_frameDelay) {
        std::this_thread::sleep_for(m_frameDelay - elapsed);
    }
    temp = std::chrono::high_resolution_clock::now();
    m_frametime = temp - m_chrono;
    m_chrono = temp;
}

double GLWindow::getFrametime() const {
    return m_frametime.count();
}

void* GLWindow::getGLProcAdress(const char* name) {
    return (void*)glfwGetProcAddress(name);
}