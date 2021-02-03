#include <MyGL/Window.hpp>
using namespace my;

bool GLWindow::gladIsInitialized = false;
unsigned int GLWindow::instancesCount = 0;

GLWindow::GLWindow() : GLWindow(800, 600, "Default") {}

GLWindow::GLWindow(int width, int height, const std::string& title) : m_projection(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), 0.1f, 10.0f)),
p_camera(nullptr), p_window(nullptr), glContext()
{
    if (instancesCount == 0) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
            std::cerr << "ERROR::SDL2: Failed to initialize Video module" << std::endl;
            exit(EXIT_FAILURE);
        }
        SDL_GL_LoadLibrary(NULL);
    }
    instancesCount++;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    p_window = SDL_CreateWindow(
        "Default",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (p_window == nullptr) {
        std::cerr << "ERROR::SDL: Failed to create a window" << std::endl;
        exit(EXIT_FAILURE);
    }

    glContext = SDL_GL_CreateContext(p_window);

    if (!gladIsInitialized) {
        //SDL_SetMainReady();
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            std::cout << "ERROR::GLAD: Failed to initialize GLAD" << std::endl;
            exit(EXIT_FAILURE);
        }
        gladIsInitialized = true;
    }

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLWindow::~GLWindow() {
    instancesCount--;
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(p_window);
    if (instancesCount == 0) {
        SDL_Quit();
    }
}

bool GLWindow::setActive(bool activate) {
    if (SDL_GL_MakeCurrent(p_window, glContext)) {
        // TODO
        return false;
    }
    return true;
}

void GLWindow::clear(const my::Color& color) const {
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.alpha / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void GLWindow::display() const {
    SDL_GL_SwapWindow(p_window);
}

void* GLWindow::getGLProcAdress(const char* name) {
    return (void*)SDL_GL_GetProcAddress(name);
}